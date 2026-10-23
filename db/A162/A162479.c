/*
 * A162479  Expansion of 1/((1-x)*sqrt(1-4x/(1-x)^4)).
 * https://oeis.org/A162479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(n+3k, n-k) * binomial(2k, k).
 *            Recurrence (valid for n >= 5): n*a(n) + 3*(2-3n)*a(n-1) +
 *            2*(7n-13)*a(n-2) + 2*(12-5n)*a(n-3) + (5n-16)*a(n-4) +
 *            (4-n)*a(n-5) = 0 Initial values: a(0) = 1, a(1) = 3, a(2) = 17,
 *            a(3) = 105, a(4) = 671. The function caches computed terms to...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Return value immediately if already known */
    if (n < cache_len)
        return cache[n];

    /* Resize cache to hold up to index n */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failure – abort; caller should handle if needed */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_len = new_len;

    /* Initialize the five known base values if missing */
    if (old_len <= 0 && new_len > 0) cache[0] = (seq_t)1;
    if (old_len <= 1 && new_len > 1) cache[1] = (seq_t)3;
    if (old_len <= 2 && new_len > 2) cache[2] = (seq_t)17;
    if (old_len <= 3 && new_len > 3) cache[3] = (seq_t)105;
    if (old_len <= 4 && new_len > 4) cache[4] = (seq_t)671;

    /* Compute the remaining terms using the recurrence */
    for (idx_t i = old_len; i < new_len; ++i) {
        if (i < 5)   /* already set */
            continue;

        /* Coefficients of the recurrence for current n = i */
        long long coeff[5];
        coeff[0] = 3 * ((long long)2 - 3 * (long long)i);
        coeff[1] = 2 * ((long long)7 * (long long)i - 13);
        coeff[2] = 2 * ((long long)12 - 5 * (long long)i);
        coeff[3] = 5 * (long long)i - 16;
        coeff[4] = 4 - (long long)i;

        /* Sum contributions separately for positive and negative coeffs */
        seq_t sum_pos = 0;   /* positive coefficient contributions */
        seq_t sum_neg = 0;   /* absolute value of negative coefficient contributions */

        for (int j = 0; j < 5; ++j) {
            idx_t prev = i - 1 - j;          /* a(i-1), a(i-2), …, a(i-5) */
            seq_t prev_val = cache[prev];
            seq_t abs_coeff = (seq_t)(coeff[j] >= 0 ? coeff[j] : -coeff[j]);

            if (coeff[j] >= 0)
                sum_pos += prev_val * abs_coeff;
            else
                sum_neg += prev_val * abs_coeff;
        }

        /* n*a(n) = sum_neg – sum_pos  (exact division) */
        seq_t diff = sum_neg - sum_pos;
        cache[i] = diff / ((seq_t)i);
    }

    return cache[n];
}
