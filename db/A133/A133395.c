/*
 * A133395  Terms in A061725 that are of form 3*prime.
 * https://oeis.org/A133395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * q where q is prime and q = (p^2+2)/3 for some prime p
 *            ≠ 3. The function generates primes p incrementally, checks the
 *            condition, and builds the sequence on demand
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

static uint64_t next_candidate = 2;        /* next integer to test for primality */

/* Return the nth term of A133395 (1‑based index).  */
seq_t a(idx_t n)
{
    /* Ensure cache size. */
    while (cache_cnt < (size_t)n) {
        /* Find next prime p. */
        uint64_t p;
        while (1) {
            if (is_prime(next_candidate)) {
                p = next_candidate;
                next_candidate++;          /* advance for next call */
                if (p == 3)                /* skip p=3 (p^2+2 not divisible by 3) */
                    continue;
                /* Compute (p^2+2)/3 and test primality of the quotient. */
                seq_t v   = ((seq_t)p) * ((seq_t)p) + 2;
                seq_t q   = v / 3;          /* exact division because 3 | (p^2+2) */
                if (is_prime_big(q)) {
                    seq_t val = q * 3;      /* 3*q is the sequence term */
                    append_candidate(&cache, &cache_cnt, &cache_cap, val);
                    break;
                }
            }
            next_candidate++;              /* not prime or fails property */
        }
    }
    return cache[(size_t)n - 1];
}
