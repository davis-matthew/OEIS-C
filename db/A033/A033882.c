/*
 * A033882  Abundancy of the abundant or perfect numbers: m = sigma(k)-2k for
 *          n such that m >= 0.
 * https://oeis.org/A033882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(A023196(n)) – 2*A023196(n) where A023196(n) is the
 *            n-th abundant or perfect number
 * Range:     u8 n <= 57, u16 n <= 8745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of the m-values for abundant/perfect numbers */
static seq_t *abundant_vals = NULL;
static size_t abundant_len = 0;
static size_t sieve_limit  = 0;

/* recompute the list of abundant/perfect m-values up to `limit` */
static void compute_abundant_upto(size_t limit)
{
    /* compute divisor sums σ(k) up to limit */
    uint64_t *sigma = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!sigma) return;          /* allocation failure – keep old cache */

    for (size_t i = 1; i <= limit; ++i)
        for (size_t j = i; j <= limit; j += i)
            sigma[j] += i;

    /* count how many abundant or perfect numbers we have found */
    size_t count = 0;
    for (size_t k = 1; k <= limit; ++k)
        if (sigma[k] >= 2 * k) ++count;

    /* allocate array for the m-values */
    seq_t *new_vals = NULL;
    if (count > 0) {
        new_vals = (seq_t *)malloc(count * sizeof(seq_t));
        if (!new_vals) { free(sigma); return; }  /* allocation failure */
        size_t idx = 0;
        for (size_t k = 1; k <= limit; ++k)
            if (sigma[k] >= 2 * k) {
                uint64_t m = sigma[k] - 2 * k;
                new_vals[idx++] = (seq_t)m;
            }
    }

    /* replace old cache with new data */
    free(abundant_vals);
    abundant_vals = new_vals;
    abundant_len  = count;
    sieve_limit   = limit;

    free(sigma);
}

/* ensures that the cache contains at least `n` terms */
static void ensure_abundant(idx_t n)
{
    if (n <= abundant_len) return;

    size_t limit = (sieve_limit > 0) ? sieve_limit : 12000;
    while (abundant_len < (size_t)n) {
        limit *= 2;                     /* grow search range */
        compute_abundant_upto(limit);
        /* if new data couldn't be computed, break to avoid infinite loop */
        if (abundant_len == 0) break;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_abundant(n);
    return abundant_vals[(size_t)(n - 1)];
}
