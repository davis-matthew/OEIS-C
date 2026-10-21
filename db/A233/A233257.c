/*
 * A233257  Number of 2 X n 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A233257
   Formula (empirical/recurrence):
      a(1) = 3,  a(2) = 10,  a(3) = 104
      a(n) = 16*a(n-1) - 48*a(n-2)   for n > 3
*/
/* If seq_t is an arbitrary-precision class, the arithmetic operators
   (+, -, *, ...) are defined. */
/* The function is memoized; it works for any invocation order. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1] .. cache[n] will hold a(i) */
static size_t cache_cap = 0;  /* allocated capacity of cache (size_t) */
static size_t cache_len = 0;  /* highest index currently cached, 0 means none */

seq_t a(idx_t n)
{
    if (n < 1)                /* behaviour for indices below the offset is
                                 unspecified; return 0 for safety */
        return 0;

    /* Ensure the cache has room for indices up to n. */
    size_t need = (size_t)n + 1;          /* +1 because we index from 1 */
    if (need > cache_cap) {
        /* realloc may return NULL if memory cannot be allocated; the
           build harness will treat this as a fatal error. */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * need);
        if (!tmp) exit(1);                 /* allocation failed */
        cache = tmp;
        cache_cap = need;
    }

    /* Fill in any missing terms up to n. */
    size_t start = cache_len + 1;
    if (start < 1)
        start = 1;

    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)3;
        else if (i == 2)
            cache[2] = (seq_t)10;
        else if (i == 3)
            cache[3] = (seq_t)104;
        else {
            /* recurrence: a(i) = 16*a(i-1) - 48*a(i-2) */
            seq_t term1 = cache[i - 1] * (seq_t)16;
            seq_t term2 = cache[i - 2] * (seq_t)48;
            cache[i] = term1 - term2;
        }
    }

    cache_len = (size_t)n;
    return cache[n];
}
