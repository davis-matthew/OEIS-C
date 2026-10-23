/*
 * A118630  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+2401)^2 = y^2.
 * https://oeis.org/A118630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 6*a(n-9)-a(n-18)+4802 for n>18
   with initial values a(1)=0, a(2)=539, … , a(18)=35819. */
static const seq_t first[18] = {
    0,   539,   924,  1220,  1715, 2744, 3503, 4095,
   5096, 7203, 9996, 12075, 13703, 16464, 22295, 26640,
   30044, 35819
};

seq_t a(idx_t n)
{
    /* cache holds values a(1)..a(maxComputed).  cache[i] = a(i+1). */
    static seq_t *cache      = NULL;
    static idx_t maxComputed = 0;   /* number of elements stored */

    if (n == 0) return 0;          /* safety for n<1, undefined by spec */

    /* if we already know a(n), return it immediately */
    if (n <= maxComputed) return cache[n-1];

    /* need to grow cache to at least n elements */
    idx_t newSize = n;
    if (newSize < 18) newSize = 18;          /* ensure we can store 18 seeds */

    /* reallocate (or allocate) the cache */
    seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
    if (!tmp) return 0;                      /* allocation failed → 0 */
    cache = tmp;

    /* initialise any newly allocated slots */
    for (idx_t i = maxComputed; i < newSize; ++i) {
        if (i < 18) {
            cache[i] = first[i];
        } else {
            cache[i] =
                (seq_t)6 * cache[i-9] - cache[i-18] + (seq_t)4802;
        }
    }
    maxComputed = newSize;

    return cache[n-1];
}
