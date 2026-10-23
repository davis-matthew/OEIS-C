/*
 * A167197  a(6) = 7, for n >= 7, a(n) = a(n - 1) + gcd(n, a(n - 1)).
 * https://oeis.org/A167197
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + gcd(n, a(n-1)), a(6) = 7, for n >= 7
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         a(n) = 0       for n < 6   (output below the offset is irrelevant) */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* cache[i] == a(6 + i) */
static idx_t cache_size = 0;  /* number of cached terms */

/* Ensure that terms up to n are computed and cached */
static void expand_cache(idx_t n) {
    if (n < 6) return;

    idx_t needed = n - 6 + 1; /* number of terms to store up to a(n) */

    if (cache == NULL) {               /* first call */
        cache = (seq_t *)calloc((size_t)needed, sizeof(seq_t));
        if (!cache) abort();           /* out of memory */

        cache[0] = (seq_t)7;           /* a(6) */
        cache_size = needed;

        for (idx_t i = 7; i <= n; ++i) {
            seq_t prev = cache[i - 7];          /* a(i-1) */
            seq_t g    = gcd((seq_t)i, prev);
            cache[i - 6] = prev + g;            /* a(i) */
        }
    } else if (needed > cache_size) {   /* extend cache */
        cache = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
        if (!cache) abort();           /* out of memory */

        for (idx_t i = cache_size + 6; i <= n; ++i) {
            seq_t prev = cache[i - 7];
            seq_t g    = gcd((seq_t)i, prev);
            cache[i - 6] = prev + g;
        }
        cache_size = needed;
    }
}

seq_t a(idx_t n) {
    if (n < 6) return 0;                /* offset is 6; behaviour below is not required */
    expand_cache(n);
    return cache[n - 6];
}
