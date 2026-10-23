/*
 * A115335  a(0) = 3, a(1) = 5, a(2) = 1, and a(n) = (2^(1 + n) - 11*(-1)^n)/3
 *          for n > 2.
 * https://oeis.org/A115335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=5, a(2)=1, a(3)=9, a(4)=7 a(n) = (2^(1+n) -
 *            11*(-1)^n)/3 for n>2 Recurrence: a(n) = a(n-1) + 2*a(n-2) for
 *            n>4 Uses memoisation to answer any order of calls efficiently
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)7;

    /* Lazy initialisation of the cache. */
    static seq_t *cache = NULL;      /* dynamically grows as needed   */
    static idx_t cached_size = 0;   /* number of values stored */

    if (cache == NULL) {
        /* Allocate space for the first five known terms. */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)7;
        cached_size = 5;
    }

    /* Grow cache if the requested index is beyond current capacity. */
    if (n >= cached_size) {
        /* Reallocate to hold up to index n. */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed; fall back to formula (rare). */
            /* Compute 2^(1+n) using repeated doubling, always casting to seq_t. */
            seq_t pow2 = (seq_t)1 << 1;          /* 2^1 = 2 */
            for (idx_t i = 2; i <= n + 1; ++i)
                pow2 = pow2 << 1;                /* multiply by 2 */
            seq_t sign = (n & 1U) ? (seq_t)11 : -(seq_t)11; /* (-1)^n * 11 */
            return (pow2 - sign) / (seq_t)3;
        }
        cache = new_cache;
        /* Compute new terms using the recurrence. */
        for (idx_t i = cached_size; i <= n; ++i) {
            /* a(i) = a(i-1) + 2*a(i-2) */
            cache[i] = cache[i - 1] + (cache[i - 2] << 1);
        }
        cached_size = n + 1;
    }

    return cache[n];
}
