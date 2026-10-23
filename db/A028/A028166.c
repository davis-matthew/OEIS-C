/*
 * A028166  Expansion of 1/((1-5x)(1-6x)(1-7x)(1-9x)).
 * https://oeis.org/A028166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 269*a(n-2) + 1173*a(n-3) - 1890*a(n-4);
 *            initial values: a(0)=1, a(1)=27, a(2)=460, a(3)=6330
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/seq_realloc.h"  /* seq_realloc */

/* Cache of already computed terms. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* First four terms (cast to seq_t at run‑time). */
static const idx_t init_vals[4] = { 1, 27, 460, 6330 };

/* Expand the cache so that it contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;                     /* already sufficient */

    idx_t new_cap = n + 1;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    /* std::realloc replacement that handles construction/destruction of
       seq_t when necessary.  The build guarantees that it returns a
       non‑NULL pointer. */
    cache = new_cache;

    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i < 4) {
            cache[i] = (seq_t)init_vals[i];
        } else {
            seq_t term1 = (seq_t)27  * cache[i - 1];
            seq_t term2 = (seq_t)269 * cache[i - 2];
            seq_t term3 = (seq_t)1173 * cache[i - 3];
            seq_t term4 = (seq_t)1890 * cache[i - 4];
            cache[i] = term1 - term2 + term3 - term4;
        }
    }
    cache_cap = new_cap;
}

/* Return a(n); handles calls in any order. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
