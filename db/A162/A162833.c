/*
 * A162833  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162833: Number of reduced words of length n in the Coxeter group
   on 30 generators with relations (S_i)^2 = (S_i S_j)^3 = I.
   Generating function: (t^3 + 2t^2 + 2t + 1)/(406t^3 - 28t^2 - 28t + 1)
   Recurrence (for n ≥ 4):
        a(n) = 28 · a(n−1) + 28 · a(n−2) – 406 · a(n−3)
   Initial values:
        a(0) = 1 ,  a(1) = 30 ,  a(2) = 870 ,  a(3) = 24795
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation ------------------------------------- */
    static seq_t *cache = NULL;     /* stored terms */
    static size_t cap   = 0;        /* allocated size */
    static size_t len   = 0;        /* terms already computed up to len−1 */

    /* --- allocate / grow cache to cover index n ----------------------- */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 4) new_cap = 4;        /* at least space for first 4 terms */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* --- initialise first four terms if not already done -------------- */
    if (len < 1) { cache[0] = (seq_t)1;     len = 1; }
    if (len < 2) { cache[1] = (seq_t)30;    len = 2; }
    if (len < 3) { cache[2] = (seq_t)870;   len = 3; }
    if (len < 4) { cache[3] = (seq_t)24795; len = 4; }

    /* --- if the requested index is already known return it ------------- */
    if ((size_t)n < len) return cache[(size_t)n];

    /* --- compute missing terms up to n --------------------------------- */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)(28 * cache[i-1] + 28 * cache[i-2] - 406 * cache[i-3]);
        cache[i] = val;
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
