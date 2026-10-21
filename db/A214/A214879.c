/*
 * A214879  Numbers that cannot be written as sum of the squares of two
 *          primes.
 * https://oeis.org/A214879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth nonnegative integer that cannot be expressed as
 *            the sum of the squares of two primes
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(n)) for n>=0. */
static idx_t int_sqrt(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Return the nth term (1‑based) of sequence A214879. */
seq_t a(idx_t n)
{
    /* Allowed: n >= 1. For n==0 we return 0 as a safe default. */
    if (n == 0) return 0;

    /* Initial upper bound for the numbers we will test. */
    idx_t limit = (n < 10 ? 10 : n * 2 + 100);

    while (true) {
        /* Boolean array marking representable numbers. */
        bool *repr = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        if (!repr) exit(1); /* Out of memory. */

        /* ---- Sieve primes up to sqrt(limit) ---- */
        idx_t s = int_sqrt(limit);
        bool *is_composite = (bool *)calloc((size_t)(s + 1), sizeof(bool));
        if (!is_composite) { free(repr); exit(1); }

        uint64_t *primes = (uint64_t *)malloc((size_t)(s + 1) * sizeof(uint64_t));
        if (!primes) { free(repr); free(is_composite); exit(1); }

        size_t prime_cnt = 0;
        for (idx_t i = 2; i <= s; ++i) {
            if (!is_composite[i]) {
                primes[prime_cnt++] = i;
                if (i * i <= s) {
                    for (idx_t j = i * i; j <= s; j += i)
                        is_composite[j] = true;
                }
            }
        }
        free(is_composite);

        /* ---- Mark all sums p^2 + q^2 <= limit ---- */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pi = primes[i];
            uint64_t pi2 = pi * pi;
            for (size_t j = i; j < prime_cnt; ++j) {
                uint64_t pj = primes[j];
                uint64_t sum = pi2 + pj * pj;
                if (sum > limit) break;
                repr[(size_t)sum] = true;
            }
        }

        free(primes);

        /* ---- Collect the first n non‑representable numbers ---- */
        seq_t *terms = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!terms) { free(repr); exit(1); }

        size_t cnt = 0;
        for (idx_t num = 0; num <= limit && cnt < n; ++num) {
            if (!repr[(size_t)num]) {
                terms[cnt++] = (seq_t)num;
            }
        }

        free(repr);

        if (cnt >= n) {
            seq_t result = terms[n - 1];
            free(terms);
            return result;
        }

        /* Not enough terms: increase the search limit and try again. */
        free(terms);
        limit = limit * 2 + 100;
    }
}
