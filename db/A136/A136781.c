/*
 * A136781  Number of multiplex juggling sequences of length n, base state
 *          <2,2> and hand capacity 2.
 * https://oeis.org/A136781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3, a(3)=21, a(4)=162;
   a(n)=14*a(n-1)-54*a(n-2)+57*a(n-3) for n>4 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values */
static idx_t  capacity = 0;          /* current allocated size (>=1) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 – return 0 */

    /* Ensure the cache can hold indices up to n (index 1 … n) */
    if (n + 1 > capacity) {
        size_t oldcap = capacity;
        size_t newcap = (capacity == 0) ? 16 : capacity * 2;
        while (newcap < (size_t)n + 1) newcap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failed – return 0 */

        cache = tmp;
        capacity = newcap;

        /* initialise newly allocated elements to zero */
        for (size_t i = oldcap; i < capacity; ++i)
            cache[i] = 0;
    }

    /* initialise base values on first use */
    if (cache[1] == 0) {
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 21;
        cache[4] = 162;
    }

    /* compute missing terms up to n */
    for (idx_t i = 5; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = 14 * cache[i-1] - 54 * cache[i-2] + 57 * cache[i-3];
        }
    }

    return cache[n];
}
