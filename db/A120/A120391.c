/*
 * A120391  Expansion of -x*(1+x-x^2+x^3+4*x^4) / (
 *          (x^3-2*x^2-x+1)*(x^3+2*x^2-x-1) ).
 * https://oeis.org/A120391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=0,a(1)=1,a(2)=1,a(3)=4,a(4)=6,a(5)=18
   For n>=6:  a(n) = a(n-6) - 6*a(n-4) + 5*a(n-2)
   (Derived from the rational generating function )
*/
#include <stddef.h>
#include <stdlib.h>

static const seq_t init[6] = {0,1,1,4,6,18};

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    /* extend cache to contain n */
    size_t needed = (size_t)n + 1;
    size_t old_size = cache_size;

    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache_size = needed;
        /* initialise first 6 entries if needed */
        for (size_t i = 0; i < needed && i < 6; ++i)
            cache[i] = init[i];
        for (size_t i = 6; i < needed; ++i)
            cache[i] = 0;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return cache[old_size-1]; /* allocation failure */
        cache = tmp;
        cache_size = needed;
        for (size_t i = old_size; i < needed; ++i)
            cache[i] = 0;
    }

    /* compute missing terms */
    for (size_t i = old_size; i <= (size_t)n; ++i) {
        if (i < 6) {
            cache[i] = init[i];
        } else {
            seq_t t1 = cache[i-6];
            seq_t t2 = cache[i-2] * (seq_t)5;
            seq_t t3 = cache[i-4] * (seq_t)6;
            cache[i] = t1 + t2 - t3;
        }
    }

    return cache[(size_t)n];
}
