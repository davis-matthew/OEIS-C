/*
 * A223374  3 X 3 square grid graph coloring a rectangular array: number of n
 *          X 3 0..8 arrays where 0..8 label nodes of the square grid graph
 *          and every array movement to a horizontal or vertical neighbor
 *          moves along an edge of this graph.
 * https://oeis.org/A223374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * a(n) = 221*a(n-2) - 1718*a(n-4)
 *            + 1872*a(n-6) for n>7 * initial values: * a(1)=68, a(2)=912,
 *            a(3)=13232, a(4)=192752, * a(5)=2812608, a(6)=41044048,
 *            a(7)=598980512 * The function implements a memoised linear‑time
 *            recurrence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
 
static seq_t *cache = NULL;          /* cache[0] unused, indices 1…n */
static size_t cache_len = 0;        /* highest index for which cache[i] is valid */
static size_t cache_cap = 0;        /* allocated capacity (>= cache_len+1) */
 
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not part of the OEIS offset – harmless */
 
    /* initialise cache with the base values if not yet done */
    if (cache_len == 0) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();                     /* allocation failure */
 
        /* set unused entry 0 to 0 */
        cache[0] = (seq_t)0;
 
        /* base terms: indices 1–7 */
        cache[1] = (seq_t)68;
        cache[2] = (seq_t)912;
        cache[3] = (seq_t)13232;
        cache[4] = (seq_t)192752;
        cache[5] = (seq_t)2812608;
        cache[6] = (seq_t)41044048;
        cache[7] = (seq_t)598980512;
 
        /* fill remaining entries up to capacity with zeros (not necessary) */
        for (size_t i = 8; i < cache_cap; ++i) cache[i] = (seq_t)0;
        cache_len = 7;
    }
 
    /* extend cache capacity if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
 
    /* compute missing values */
    if ((size_t)n > cache_len) {
        idx_t start = cache_len + 1;
        if (start < 8) start = 8;                /* recurrence applies from n=8 */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)221 * cache[i - 2]
                       - (seq_t)1718 * cache[i - 4]
                       + (seq_t)1872 * cache[i - 6];
        }
        cache_len = n;
    }
 
    return cache[n];
}
