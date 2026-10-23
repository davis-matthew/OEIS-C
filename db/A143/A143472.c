/*
 * A143472  Expansion of 1/(1 - x^3 - x^5 - x^7 + x^10), inverse of a Salem
 *          polynomial.
 * https://oeis.org/A143472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5) + a(n-7) - a(n-10)
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 113, u64 n <= 220,
 *            u128 n <= 434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=1,
   a(6)=1, a(7)=1, a(8)=2, a(9)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed values   */
    static size_t cache_size = 0;    /* number of elements stored */

    /* Ensure the cache is large enough for indices 0..n inclusive */
    size_t need = (size_t)n + 1;
    if (need > cache_size) {
        size_t old_size = cache_size;
        /* Allocate (or re‑allocate) a larger array */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – abort (unlikely for moderate n) */
            abort();
        }
        cache = tmp;
        cache_size = need;

        /* Fill in the new entries */
        for (size_t i = old_size; i < need; ++i) {
            if (i < 10) {  /* initial conditions */
                switch (i) {
                    case 0: cache[i] = (seq_t)1; break;
                    case 1: cache[i] = (seq_t)0; break;
                    case 2: cache[i] = (seq_t)0; break;
                    case 3: cache[i] = (seq_t)1; break;
                    case 4: cache[i] = (seq_t)0; break;
                    case 5: cache[i] = (seq_t)1; break;
                    case 6: cache[i] = (seq_t)1; break;
                    case 7: cache[i] = (seq_t)1; break;
                    case 8: cache[i] = (seq_t)2; break;
                    case 9: cache[i] = (seq_t)1; break;
                }
            } else {       /* recurrence for i >= 10 */
                cache[i] = cache[i-3] + cache[i-5] + cache[i-7] - cache[i-10];
            }
        }
    }

    return cache[n];
}
