/*
 * A224659  Number of n X n 0..2 matrices with each 2 X 2 subblock idempotent.
 * https://oeis.org/A224659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 5*a(n-3) + 12*a(n-4) - 12*a(n-5) -
 *            3*a(n-6) + 6*a(n-7) - a(n-9), with a(1)=3, a(2)=12, a(3)=50,
 *            a(4)=108, a(5)=260, a(6)=542, a(7)=1126, a(8)=2230, a(9)=4336,
 *            a(10)=8246
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cache[0] is unused, cache[i] = a(i) */
static size_t cache_len = 0;          /* allocated length */
static idx_t max_filled = 0;          /* largest n whose value is stored */

/* Ensure that cache can hold indices up to n (inclusive) and that
   all terms 1..n are computed. */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache array. */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) abort();          /* unrecoverable allocation failure */
        cache = new_cache;
        cache_len = new_len;
    }

    /* Initial values for the first ten terms. */
    static const seq_t init[10] = {
        (seq_t)3,    (seq_t)12,   (seq_t)50,   (seq_t)108,
        (seq_t)260,  (seq_t)542,  (seq_t)1126, (seq_t)2230,
        (seq_t)4336, (seq_t)8246
    };

    /* Compute any missing terms up to n. */
    for (idx_t i = max_filled + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 10) {
            val = init[(size_t)(i - 1)];
        } else {
            seq_t pos = 0, neg = 0;
            pos += (seq_t)6 * cache[(size_t)(i - 1)];
            pos += (seq_t)5 * cache[(size_t)(i - 3)];
            pos += (seq_t)12 * cache[(size_t)(i - 4)];
            pos += (seq_t)6 * cache[(size_t)(i - 7)];
            neg += (seq_t)12 * cache[(size_t)(i - 2)];
            neg += (seq_t)12 * cache[(size_t)(i - 5)];
            neg += (seq_t)3 * cache[(size_t)(i - 6)];
            neg += cache[(size_t)(i - 9)];
            val = pos - neg;    /* pos ≥ neg for all n ≥ 11 */
        }
        cache[(size_t)i] = val;
    }
    max_filled = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; undefined for n = 0 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)n];
}
