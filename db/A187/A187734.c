/*
 * A187734  a(n) is the number of n-walks between the vertices 1 and 3 of the
 *          Graph on the chalkboard in 'Good Will Hunting', (1997).
 * https://oeis.org/A187734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187734: number of n‑walks between vertices 1 and 3
   Recurrence: a(1)=0, a(2)=a(3)=2,
                a(n)=a(n‑1)+6·a(n‑2)–4·a(n‑3) for n≥4
   (derived from the generating function 2·x²/(1–x–6·x²+4·x³)) */

#include <stdlib.h>

static idx_t   max_computed = 0;   /* highest n already filled in cache   */
static seq_t * cache        = NULL;/* cache[0] unused, cache[n] = a(n)     */

seq_t a(idx_t n)
{
    /* defined only for n≥1 – return 0 for non‑positive indices */
    if (n < 1) return (seq_t)0;

    /* enlarge cache if required */
    if (n > max_computed) {
        size_t new_size = (size_t)(n + 1); /* allocate up to index n */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */

        /* compute values from the next missing index up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)0;
            } else if (i == 2 || i == 3) {
                cache[i] = (seq_t)2;
            } else {
                /* use only seq_t operations – works with all supported types */
                cache[i] = cache[i-1] + (seq_t)6 * cache[i-2]
                                  - (seq_t)4 * cache[i-3];
            }
        }
        max_computed = n;
    }
    return cache[n];
}
