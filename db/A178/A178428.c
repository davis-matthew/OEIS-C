/*
 * A178428  5 followed by the generalized Fermat numbers 6^(2^n)+1 (A078303).
 * https://oeis.org/A178428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(0)=5; for n≥1, a(n)=2+∏_{i=0}^{n-1}a(i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute A178428: 5 followed by the generalized Fermat numbers 6^(2^n)+1.
   The terms are pairwise coprime.  We memoise all computed values.
   The code uses only sequence arithmetic functions that are provided
   (seq_mul for multiplication).  The implementation is O(n) with
   constant‑time product updates. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"      /* seq_mul(seq_t, seq_t) -> seq_t */
#include "../utils/alloc_seq_array.h"  /* alloc_seq_array(size_t) -> seq_t* */
#include "../utils/free_seq_array.h"   /* free_seq_array(seq_t*) */

static seq_t *cache = NULL;          /* memoised terms */
static idx_t cache_cap = 0;          /* allocated capacity of cache */
static idx_t cache_len = 0;          /* number of terms that are cached */
static seq_t prod = (seq_t)1;        /* product of all cached terms */
static bool inited = false;          /* one‑time initialisation flag */

/* ensure cache has capacity at least `need` */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap) return;
    idx_t new_cap = cache_cap > 0 ? cache_cap : 4;
    while (new_cap <= need) new_cap <<= 1;   /* double until large enough */

    seq_t *new_cache = alloc_seq_array(new_cap);
    /* copy old terms */
    for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    /* initialise remaining slots to zero */
    for (idx_t i = cache_len; i < new_cap; ++i) new_cache[i] = (seq_t)0;

    if (cache) free_seq_array(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!inited) {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
        cache_len = 0;
        prod = (seq_t)1;
        inited = true;
    }

    ensure_capacity(n);

    /* extend cache up to n (inclusive) */
    while (cache_len <= n) {
        seq_t val;
        if (cache_len == 0) {          /* a(0) = 5 */
            val = (seq_t)5;
        } else {                       /* a(k) = 2 + product of previous terms */
            val = prod + (seq_t)2;
        }
        cache[cache_len] = val;
        prod = seq_mul(prod, val);     /* update running product */
        ++cache_len;
    }

    return cache[n];
}
