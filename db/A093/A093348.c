/*
 * A093348  A 5-fractal "castle" starting with 0.
 * https://oeis.org/A093348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; for n>=2, * let w(n)=5^{⌊log_5(n-1)⌋} (largest power of
 *            5 ≤ n-1), * then a(n)=w(n)-a(n-w(n))
 * Range:     u8 n <= 625, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   w_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Allow both C11 and C++17 builds */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/w_of.h"

/* Forward declarations used only by this file */
static idx_t w_of(idx_t n);

/* ------------------------------------------------------------------ */
/*  Cache for computed values.  1‑based indexing: cache[1] == a(1).   */
/* ------------------------------------------------------------------ */
static seq_t *cache     = NULL;  /* pointer to array of seq_t */
static idx_t  cache_sz = 0;     /* number of computed entries (last index) */

/* ------------------------------------------------------------------ */
/*  w_of: largest power of five not exceeding n-1.                     */

/* ------------------------------------------------------------------ */
/*  a:  Return the n‑th term of A093348 (offset 1).  The sequence is  */
/*      cached; calls may arrive in any order.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {                /* safety for out‑of‑range callers */
        return (seq_t)0;
    }

    /* Make sure the cache is large enough */
    if (n > cache_sz) {
        idx_t new_sz = n + 1;    /* need indices 0..n; 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) {              /* allocation failed – unlikely in tests */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise the new part of the cache */
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)0;        /* a(1)=0 */
            } else {
                idx_t w = w_of(i);
                cache[i] = (seq_t)w - cache[i - w];
            }
        }
        cache_sz = n;
    }
    return cache[n];
}
