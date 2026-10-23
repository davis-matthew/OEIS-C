/*
 * A025252  a(n) = (1/2)*s(n+3), where s = A025251.
 * https://oeis.org/A025252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*s(n+3) where s = A025251. Recurrence used (valid
 *            for n ≥ 5 with a0 = 1): (n+3)a(n)+(n+2)a(n-1)-2n
 *            a(n-2)+2(-5n+7)a(n-3)+(-7n+17)a(n-4)+(n-4)a(n-5)=0 Initial
 *            values (offset 1): a(1)=0, a(2)=1, a(3)=2, a(4)=1, a(5)=6. Extra
 *            a(0)=1 is used only for the recurrence
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* array storing computed values */
static size_t cache_len = 0;           /* number of stored elements (indices 0..len-1) */

seq_t a(idx_t n)
{
    /* Ensure cache is initialized with the base values up to a(4). */
    if (cache_len == 0) {
        /* Allocate space for indices 0..4 (5 elements). */
        cache_len = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();

        /* Base values.  a(0) is only used for the recurrence. */
        cache[0] = (seq_t)1;   /* a0 = 1  */
        cache[1] = (seq_t)0;   /* a1 = 0  */
        cache[2] = (seq_t)1;   /* a2 = 1  */
        cache[3] = (seq_t)2;   /* a3 = 2  */
        cache[4] = (seq_t)1;   /* a4 = 1  */
    }

    /* If the requested index is beyond the current cache, extend it. */
    if ((size_t)n >= cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_len = new_len;

        /* Compute missing terms using the recurrence. */
        for (size_t i = old_len; i < new_len; ++i) {
            /* Compute each term of the numerator. */
            seq_t term1 = ((seq_t)((idx_t)(i + 2))) * cache[i - 1];
            seq_t term2 = ((seq_t)((idx_t)(2 * i))) * cache[i - 2];
            seq_t coeff3 = (seq_t)((idx_t)(5 * i - 7u));
            coeff3 = coeff3 * (seq_t)2;
            seq_t term3 = coeff3 * cache[i - 3];
            seq_t coeff4 = (seq_t)((idx_t)(7 * i - 17u));
            seq_t term4 = coeff4 * cache[i - 4];
            seq_t term5 = ((seq_t)((idx_t)(i - 4))) * cache[i - 5];

            /* Sum of positive terms. */
            seq_t sumPos = term2 + term3 + term4;
            /* Sum of negative terms. */
            seq_t toSub = term1 + term5;

            /* Numerator is the difference, guaranteed non‑negative. */
            seq_t num = sumPos - toSub;
            seq_t denom = (seq_t)((idx_t)(i + 3));

            cache[i] = num / denom;
        }
    }

    /* Return the requested value. */
    return cache[(size_t)n];
}
