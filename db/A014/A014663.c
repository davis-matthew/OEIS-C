/*
 * A014663  Primes p such that multiplicative order of 2 modulo p is odd.
 * https://oeis.org/A014663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014663: primes p such that the multiplicative order of 2 modulo p is odd */
/* (equivalently, p does not divide 2^n+1 for any n) */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/powmod64.h"   /* for uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod) */

static void grow_uint64(uint64_t **arr, size_t *cnt, size_t *cap)
{
    if (*cnt >= *cap) {
        *cap = (*cap == 0) ? 16 : *cap * 2;
        *arr = (uint64_t *)realloc(*arr, (*cap) * sizeof(uint64_t));
    }
}

static void grow_seq(uint64_t **arr, size_t *cnt, size_t *cap)    /* used for seq_t (cast to uint64_t) */
{
    if (*cnt >= *cap) {
        *cap = (*cap == 0) ? 16 : *cap * 2;
        *arr = (uint64_t *)realloc(*arr, (*cap) * sizeof(uint64_t));
    }
}

/* Static state for memoization */
static uint64_t *prime_list = NULL;       /* all primes found so far */
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

static uint64_t *good_primes = NULL;      /* primes satisfying the OEIS condition */
static size_t good_cnt   = 0;
static size_t good_cap   = 0;

static uint64_t last_checked = 1;          /* last odd number tested, 1 -> start at 3 */

/* Return true if x is prime. Uses trial division by previously found primes. */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    uint64_t sqrtx = (uint64_t) sqrt((double) x);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrtx; ++i) {
        if (x % prime_list[i] == 0) return false;
    }
    return true;
}

/* Append prime 'p' to prime_list */
static void add_prime(uint64_t p)
{
    grow_uint64(&prime_list, &prime_cnt, &prime_cap);
    prime_list[prime_cnt++] = p;
}

/* Append good prime 'p' to good_primes */
static void add_good_prime(uint64_t p)
{
    grow_seq(&good_primes, &good_cnt, &good_cap);
    good_primes[good_cnt++] = p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, return 0 as placeholder */

    /* Ensure we have at least n good primes */
    while (good_cnt < (size_t)n) {
        /* Find next odd candidate after the last checked */
        uint64_t cand = last_checked + 2;
        while (true) {
            if (is_prime(cand)) {
                add_prime(cand);
                /* Test the OEIS condition: 2^k mod cand == 1, where k is odd part of cand-1 */
                uint64_t k = cand - 1;
                while ((k & 1ULL) == 0) k >>= 1;      /* odd part of cand-1 */
                if (powmod64(2ULL, k, cand) == 1ULL) {
                    add_good_prime(cand);
                }
                last_checked = cand;
                break;
            } else {
                cand += 2;   /* skip even numbers */
            }
        }
    }

    /* Return the (n-1)-th element (zero‑based) */
    return (seq_t) good_primes[n - 1];
}
