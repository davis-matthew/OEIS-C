/*
 * A029119  Expansion of 1/((1-x)(1-x^7)(1-x^8)(1-x^11)).
 * https://oeis.org/A029119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-7)-a(n-9)+a(n-11)-a(n-12)-a(n-15)+a(n-16)-a(n-18)+a(n-20)+a(n-26)-a(n-27).
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1,
 *            a(6)=1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;        /* array of computed values */
static size_t cache_size = 0;      /* number of entries actually in use (>=1) */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure that cache contains entries 0..need.  If need is larger than the
   current cache, grow or allocate as necessary and fill missing terms. */
static void ensure_cache(idx_t need)
{
    /* first allocation and seed the known initial values */
    if (cache == NULL) {
        cache_cap = 8;
        cache = seq_alloc_new_array(cache_cap);
        if (cache == NULL) return;                /* allocation failure – give up */
        for (idx_t i = 0; i <= 6 && (size_t)i < cache_cap; ++i)
            cache[i] = (seq_t)1;
        cache_size = 7;                          /* indices 0..6 populated */
    }

    if ((size_t)need < cache_size) return;       /* already have required range */

    /* grow capacity if needed, doubling until sufficient */
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)need + 1) new_cap <<= 1;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (tmp == NULL) return;                /* allocation failure – give up */
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        seq_free_array(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to 'need' using the recurrence */
    for (idx_t i = cache_size; i <= need; ++i) {
        seq_t pos = 0, neg = 0;
        if (i >= 1)  pos += cache[i-1];
        if (i >= 7)  pos += cache[i-7];
        if (i >= 11) pos += cache[i-11];
        if (i >= 16) pos += cache[i-16];
        if (i >= 20) pos += cache[i-20];
        if (i >= 26) pos += cache[i-26];
        if (i >= 9)  neg += cache[i-9];
        if (i >= 12) neg += cache[i-12];
        if (i >= 15) neg += cache[i-15];
        if (i >= 18) neg += cache[i-18];
        if (i >= 27) neg += cache[i-27];
        cache[i] = pos - neg;
    }
    cache_size = (size_t)need + 1;
}

/* Public sequence function.  Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                  /* fast return for the trivial case */
    ensure_cache(n);
    return cache[n];
}
