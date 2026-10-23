/*
 * A018912  Expansion of 1/((1-4x)(1-5x)(1-10x)).
 * https://oeis.org/A018912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 110*a(n-2) + 200*a(n-3), a(0)=1, a(1)=19,
 *            a(2)=251
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Cache for computed terms.  Dynamically resized to accommodate any      */
/* requested n.  The recurrence uses only previous three values, so     */
/* values are computed on demand and cached for future calls.            */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* array of terms          */
static size_t cache_len = 0;     /* number of terms stored  */
static size_t cache_cap = 0;     /* allocated capacity      */
#define INIT_CAP 3

static void ensure_cache(idx_t n)
{
    /* Ensure the array can hold term n. */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : INIT_CAP;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* On first call initialise first three terms. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)251;
        cache_len = 3;
    }

    /* Compute missing terms up to n. */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)19   * cache[i-1]
                     - (seq_t)110  * cache[i-2]
                     + (seq_t)200  * cache[i-3];
        }
        cache_len = (size_t)n + 1;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
