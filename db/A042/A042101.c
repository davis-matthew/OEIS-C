/*
 * A042101  Denominators of continued fraction convergents to sqrt(574).
 * https://oeis.org/A042101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 1150*a(n-4) - a(n-8) for n>7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=23, a(3)=24, a(4)=1127, a(5)=1151, a(6)=26449,
 *            a(7)=27600
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static idx_t cache_len = 0;      /* number of terms stored */

/* Return a(n) – denominator of the convergents to sqrt(574) */
seq_t a(idx_t n)
{
    /* Initialise cache with the first 8 terms if not already done */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 23;
        cache[3] = 24;
        cache[4] = 1127;
        cache[5] = 1151;
        cache[6] = 26449;
        cache[7] = 27600;
    }

    /* Grow cache if the requested index is beyond what we have */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        for (idx_t i = old_len; i < cache_len; ++i) {
            /* i ≥ 8, so i-4 and i-8 are valid indices */
            cache[i] = 1150 * cache[i - 4] - cache[i - 8];
        }
    }

    return cache[n];
}
