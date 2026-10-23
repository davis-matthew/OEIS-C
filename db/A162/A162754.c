/*
 * A162754  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(2)=56, a(3)=364;
   for n>=4, a(n)=6*a(n-1)+6*a(n-2)-21*a(n-3)      */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cached values      */
static size_t cache_cap = 0;    /* current capacity    */
static size_t cache_used = 0;   /* highest index computed + 1 */

seq_t a(idx_t n) {
    size_t idx = (size_t)n;

    /* Grow the cache if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= idx) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx */
    while (cache_used <= idx) {
        switch (cache_used) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)8; break;
            case 2: cache[2] = (seq_t)56; break;
            case 3: cache[3] = (seq_t)364; break;
            default: {
                seq_t a1 = cache[cache_used - 1];
                seq_t a2 = cache[cache_used - 2];
                seq_t a3 = cache[cache_used - 3];
                seq_t val = a1 * (seq_t)6 + a2 * (seq_t)6 - a3 * (seq_t)21;
                cache[cache_used] = val;
            } break;
        }
        ++cache_used;
    }

    return cache[idx];
}
