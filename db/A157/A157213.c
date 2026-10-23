/*
 * A157213  Positive numbers y such that y^2 is of the form x^2+(x+137)^2 with
 *          integer x.
 * https://oeis.org/A157213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A157213: Positive numbers y such that y^2 = x^2 + (x+137)^2
   Recurrence: a(n) = 6*a(n-3) - a(n-6)  (n > 6)
   Initial terms: a(1)=97, a(2)=137, a(3)=277, a(4)=305, a(5)=685, a(6)=1565 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;          /* array holding computed a(i) for i=1..cache_len */
static size_t cache_len = 0;         /* number of terms currently stored   */
static size_t cache_cap = 0;         /* current capacity of the array       */

/* Compute a(n) with memoisation and the recurrence relation */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset < 1, return 0 */

    size_t idx = (size_t)n;              /* convert to size_t for indexing */
    /* initialise cache on first use */
    if (cache_len == 0) {
        /* allocate enough space for at least the first 6 terms */
        cache_cap = idx > 8 ? idx : 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* store the first six terms */
        cache[0] = 97;
        cache[1] = 137;
        cache[2] = 277;
        cache[3] = 305;
        cache[4] = 685;
        cache[5] = 1565;
        cache_len = 6;
    }

    /* grow capacity if the requested index exceeds it */
    if (idx > cache_cap) {
        cache_cap = idx;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute missing terms up to idx */
    if (idx > cache_len) {
        for (size_t i = cache_len + 1; i <= idx; ++i) {
            /* use 1‑based indices in the recurrence:
               a(i) = 6*a(i-3) - a(i-6) */
            cache[i - 1] = 6 * cache[i - 4] - cache[i - 7];
        }
        cache_len = idx;
    }

    return cache[idx - 1];
}
