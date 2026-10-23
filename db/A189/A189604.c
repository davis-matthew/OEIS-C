/*
 * A189604  Number of n X 3 array permutations with each element not moving,
 *          or moving one space E, S or NW.
 * https://oeis.org/A189604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2); a(1)=1, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t len   = 0;  /* number of terms currently stored (len >= 0) */

    if (n == 0) return (seq_t)0;   /* safeguard – not needed for the sequence */

    if (n <= len)
        return cache[n-1];          /* already computed */

    /* initialise or grow the cache capacity */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }
    while (cap < n) {              /* double until sufficient */
        cap <<= 1;
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms from len+1 up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else if (i == 2) {
            cache[1] = (seq_t)6;
        } else {
            cache[i-1] = (seq_t)((seq_t)3 * cache[i-2] + (seq_t)2 * cache[i-3]);
        }
    }
    len = n;                        /* update stored length */

    return cache[n-1];
}
