/*
 * A101988  Number of primes (with repetition) that can be formed from digits
 *          of the n-th prime.
 * https://oeis.org/A101988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the total number of prime numbers that can be formed *
 *            from some or all decimal digits of the n‑th prime, * counting
 *            each construction separately (different choices of * which digit
 *            positions are used or the order of the chosen digits * are
 *            counted even if they produce the same prime)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/*  Dynamically generate the list of primes up to a required count   */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;   /* array of discovered primes */
static size_t    prime_cap   = 0;       /* current capacity of prime_list   */
static size_t    prime_n    = 0;       /* number of primes stored          */

static void build_primes_up_to(size_t need)
{
    if (prime_n >= need)           /* already have enough primes */
        return;

    if (!prime_cap) {
        prime_cap = need > 16 ? need : 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    /* simple primality test; primes are added in increasing order */
    uint64_t candidate = (prime_n == 0) ? 2 : prime_list[prime_n - 1] + 1;
    for (;;) {
        if (candidate == 2 || candidate % 2) {          /* odd numbers only */
            if (is_prime(candidate)) {
                if (prime_n == prime_cap) {            /* grow array */
                    prime_cap <<= 1;
                    prime_list = (uint64_t *)realloc(
                        prime_list, prime_cap * sizeof(uint64_t));
                }
                prime_list[prime_n++] = candidate;
                if (prime_n >= need)
                    break;
            }
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Generate all primes from a chosen multiset of digits (counts)     */
/* ------------------------------------------------------------------ */
static void enumerate_permutations(int *cnt, int len, uint64_t cur, seq_t *out)
{
    if (len == 0) {
        if (cur >= 2 && is_prime(cur))
            (*out)++;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (cnt[d]) {
            cnt[d]--;
            enumerate_permutations(cnt, len - 1, cur * 10 + d, out);
            cnt[d]++;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence offset is 1 */
        return 0;

    /* 1. obtain the n-th prime.  */
    build_primes_up_to(n);
    uint64_t p = prime_list[n - 1];

    /* 2. extract its decimal digits into an array.  */
    int tmp = (int)p;
    int digits[20];                 /* 20 digits are more than enough for 64-bit */
    int d_len = 0;
    if (tmp == 0) {
        digits[d_len++] = 0;
    } else {
        while (tmp) {
            digits[d_len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* 3. enumerate all non‑empty subsets of the digit positions.  */
    seq_t result = 0;
    uint32_t total_masks = 1u << d_len;
    for (uint32_t mask = 1; mask < total_masks; ++mask) {
        /* build digit count array for this subset */
        int cnt[10] = {0};
        int sz = 0;
        for (int i = 0; i < d_len; ++i)
            if ((mask >> i) & 1u) {
                ++cnt[digits[i]];
                ++sz;
            }
        enumerate_permutations(cnt, sz, 0, &result);
    }
    return result;
}
