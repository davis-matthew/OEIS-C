/*
 * A038261  First line of 5-wave sequence A038201, also bisection of A006358.
 * https://oeis.org/A038261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 35*a(n-2) + 28*a(n-3) - 9*a(n-4) + a(n-5) /*
 *            Initial values: a(0)=1, a(1)=5, a(2)=55, a(3)=671, a(4)=8272
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t last_computed = 0;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Allocate and initialise cache on first call */
    if (cache == NULL) {
        cache_size = (size_t)((n >= 4) ? (n + 1) : 5);
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)55;
        cache[3] = (seq_t)671;
        cache[4] = (seq_t)8272;
        last_computed = 4;
    } else if ((size_t)(n + 1) > cache_size) {
        size_t newsize = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        for (size_t i = cache_size; i < newsize; ++i)
            cache[i] = (seq_t)0;
        cache_size = newsize;
    }

    /* Compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t pos = cache[i-1] * (seq_t)15 + cache[i-3] * (seq_t)28 + cache[i-5];
            seq_t sub = cache[i-2] * (seq_t)35 + cache[i-4] * (seq_t)9;
            cache[i] = pos - sub;
        }
        last_computed = n;
    }

    return cache[n];
}
