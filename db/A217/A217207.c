/*
 * A217207  Start with a(1)=0; then a(n) = (curling number of
 *          (a(1),...,a(n-1))) mod 2.
 * https://oeis.org/A217207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (curling number of (a(1),...,a(n-1))) mod 2; a(1)=0
 * Range:     u8 n <= 150, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   curling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/curling.h"

/* static cache of already computed terms */
static seq_t *seq_cache = NULL;
static idx_t  cache_cap   = 0;   /* current allocated capacity   */
static idx_t  cache_used  = 0;   /* number of terms computed     */

/* ensure that cache can hold at least 'n' terms (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = n;
    seq_t *new_mem = (seq_t *)realloc(seq_cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_mem && new_cap > 0) abort();   /* memory exhausted */ 
    seq_cache = new_mem;
    cache_cap = new_cap;
}

/* main entry function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not part of the OEIS range */

    /* prepare cache big enough for n terms */
    ensure_cache(n);

    /* initialise first term if cache empty */
    if (cache_used == 0) {
        seq_cache[0] = 0;                  /* a(1) = 0 */
        cache_used   = 1;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_used; i < n; ++i) {
        /* compute a(i+1): curling number of first i terms */
        seq_t curl = curling(seq_cache, i);
        seq_cache[i] = curl & 1;           /* modulo 2 */
    }
    cache_used = n;

    return seq_cache[n - 1];
}
