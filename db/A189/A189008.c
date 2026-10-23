/*
 * A189008  Zero-one sequence based on cubes: a(A000578(k))=a(k);
 *          a(A007412(k))=1-a(k); a(1)=0.
 * https://oeis.org/A189008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Self‑generating zero‑one sequence based on cubes:
   a(1) = 0;
   for n > 1:
     if n is a perfect cube (n = k³) then a(n) = a(k);
     else a(n) = 1 - a(n - ⌊∛n⌋)                     */

#include <stdlib.h>
#include "../utils/int_cube_root.h"

static seq_t *cache = NULL;     /* memoisation array */
static idx_t  cache_cap = 0;    /* allocated size of cache   */
static idx_t  max_comp   = 0;    /* largest n for which a(n) is computed */

/* Return a(n) for n >= 1, memoising results. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1; guard for 0 */

    /* Ensure capacity for n+1 indices (0..n) */
    idx_t need_cap = n + 1;
    if (need_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, need_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = need_cap;
    }

    /* Initialise base case if not already done */
    if (max_comp == 0) {
        cache[1] = 0;            /* a(1) = 0 */
        max_comp = 1;
    }

    /* If requested index already computed, return it */
    if (n <= max_comp) return cache[n];

    /* Compute values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        idx_t r = int_cube_root(i);       /* floor cube root of i */
        idx_t r3 = r * r * r;
        if (r3 == i)                      /* i == r³  → a(i) = a(r) */
            cache[i] = cache[r];
        else                              /* non‑cube  → a(i) = 1 - a(i-r) */
            cache[i] = (seq_t)1 - cache[i - r];
    }
    max_comp = n;

    return cache[n];
}
