/*
 * A124026  Let M = {{0, -1, 2}, {-1, 2, -1}, {2, -1, 0}}; v[1] = {0, 0, 1};
 *          v[n] = M.v[n - 1]; then a(n) = v[n][[1]].
 * https://oeis.org/A124026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124026
   Recurrence:  a(1)=0, a(2)=2, a(3)=1
                a(n)=2*a(n-1)+6*a(n-2)-4*a(n-3)   for n>=4
   (OEIS offset 1)
*/

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static size_t cap   = 0;      /* number of allocated entries (>= 4) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* out of range – not used */

    /* --- initialise first three values on first call --- */
    if (!cache) {
        /* allocate space for indices 0..3 (index 0 unused) */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;   /* dummy, never accessed */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* --- grow cache if necessary --- */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;   /* enough for index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute missing terms in order */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            cache[i] = 2 * a1 + 6 * a2 - 4 * a3;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
