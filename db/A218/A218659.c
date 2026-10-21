/*
 * A218659  Number of n X 4 binary arrays with every 0 a king-move neighbor to
 *          some 1.
 * https://oeis.org/A218659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) +45*a(n-2) +180*a(n-3) -27*a(n-4) -81*a(n-6)
 *            initial: a(1)=9, a(2)=225, a(3)=3249, a(4)=50625, a(5)=793881,
 *            a(6)=12383361 (n≥7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Validate index */
    if (n == 0) return (seq_t)0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;                           /* arbitrary initial size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure – return 0 as safe fallback */
            return (seq_t)0;
        }
        /* Base values (1‑indexed) */
        cache[0] = (seq_t)9;
        cache[1] = (seq_t)225;
        cache[2] = (seq_t)3249;
        cache[3] = (seq_t)50625;
        cache[4] = (seq_t)793881;
        cache[5] = (seq_t)12383361;
    }

    /* Ensure we can hold values up to n */
    size_t need = (size_t)n;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* reallocation failed – return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return known values directly */
    if (n <= 6) return cache[(size_t)(n) - 1];

    /* Compute missing values on demand */
    for (idx_t i = 7; i <= n; ++i) {
        /* indices: i-1 holds a(i), i-2 holds a(i-1), etc. */
        seq_t val = (seq_t)12 * cache[(size_t)(i - 2)]
                  + (seq_t)45 * cache[(size_t)(i - 3)]
                  + (seq_t)180 * cache[(size_t)(i - 4)]
                  - (seq_t)27 * cache[(size_t)(i - 5)]
                  - (seq_t)81 * cache[(size_t)(i - 7)];
        cache[(size_t)(i - 1)] = val;
    }

    return cache[(size_t)(n) - 1];
}
