/*
 * A180147  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: (1 +
 *          3*x)/(1 - 4*x - 3*x^2 + 6*x^3).
 * https://oeis.org/A180147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 7, a(2) = 31
   a(n) = 4*a(n-1) + 3*a(n-2) - 6*a(n-3)   for n >= 3
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of stored values */
    static idx_t cache_cap = 0;  /* allocated capacity */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)31;
        cache_size = 3;
    }

    /* grow cache if needed */
    while (cache_size <= n) {
        /* increase capacity */
        if (cache_size >= cache_cap) {
            idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) return 0; /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* compute next value */
        seq_t val;
        if (cache_size == 0) {
            val = (seq_t)1;          /* a(0) */
        } else if (cache_size == 1) {
            val = (seq_t)7;          /* a(1) */
        } else if (cache_size == 2) {
            val = (seq_t)31;         /* a(2) */
        } else {
            val = (seq_t)4 * cache[cache_size-1]
                + (seq_t)3 * cache[cache_size-2]
                - (seq_t)6 * cache[cache_size-3];
        }
        cache[cache_size] = val;
        cache_size++;
    }

    return cache[n];
}
