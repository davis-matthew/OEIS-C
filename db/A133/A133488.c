/*
 * A133488  a(1) = 1. a(n) = a(n-1) + a(m), where m is the largest term of the
 *          sequence {a(k)} which is less than n.
 * https://oeis.org/A133488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n≥2, a(n)=a(n-1)+a(m) where m is the
   largest term a(k) that is < n (m is a value, not an index). */

#include <stdlib.h>
#include <stdint.h>

/* Return the n-th term of OEIS A133488. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache   = NULL;   /* array of computed terms, 1‑based */
    static size_t   cache_cap = 0;  /* allocated capacity (elements)   */
    static idx_t    cache_len = 0;  /* largest index that has been computed */
    static idx_t    pos       = 1;  /* last index with a[pos] < current k */

    /* Initialise on first call.  Allocate space for element 0 (unused). */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[1] = (seq_t)1;
        cache_len = 1;
        pos = 1;
    }

    /* If n is already known, return it immediately. */
    if (n <= cache_len)
        return cache[n];

    /* Grow the storage if necessary. */
    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = need;
    }

    /* Compute new terms sequentially from cache_len+1 up to n. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Move pos forward while next stored value is still < k. */
        while (pos + 1 <= k - 1 && cache[pos + 1] < (seq_t)k)
            ++pos;

        /* m is the value cache[pos], which is < k.    */
        seq_t val_m = cache[pos];          /* largest a(k) < k */
        idx_t idx_m = (idx_t)val_m;        /* index to use for a(m) */

        cache[k] = cache[k - 1] + cache[idx_m];
    }

    cache_len = n;
    return cache[n];
}
