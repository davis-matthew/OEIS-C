/*
 * A093705  Numbers that are divisible by the total number of 1's in the
 *          binary expansions of all their divisors.
 * https://oeis.org/A093705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1171, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_divisors, factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093705: Numbers divisible by the total number of 1's in the binary
   expansions of all their divisors.
   Formula: a(n) = smallest m ≥ a(n‑1)+1 such that
   m % (Σ_{d|m} popcount(d)) = 0.   */
/* The implementation enumerates candidates in increasing order, keeps
   a memoised list of accepted terms and returns the n‑th term. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_uint64.h"   /* factor_uint64 */
#include "../utils/add_divisors.h"

static uint64_t divisor_popcount_sum(uint64_t n)
{
    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64(n, fac, exp, &cnt);
    uint64_t sum = 0;
    add_divisors(1, 0, fac, exp, cnt, &sum);
    return sum;
}

seq_t a(idx_t n)
{
    /* Static storage for found terms. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t next_candidate = 1;   /* candidate starts from 1 */

    /* Allocate initial buffer. */
    if (!terms) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        uint64_t x = next_candidate;
        uint64_t s = divisor_popcount_sum(x);
        if (s != 0 && x % s == 0) {
            /* Store the term, expanding the buffer if needed. */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)x;
        }
        ++next_candidate;
    }

    return terms[n - 1];   /* sequence indices are 1‑based */
}
