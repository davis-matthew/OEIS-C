/*
 * A082957  Numbers k such that sigma(2*k) < sigma(2*k+1).
 * https://oeis.org/A082957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer k such that sigma(2*k) <
 *            sigma(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 1365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* 1‑based cache of found k‑values */
static seq_t *cache = NULL;      /* array, index 0 unused */
static size_t cache_cap = 0;     /* allocated capacity (max index) */
static size_t count = 0;         /* number of k's stored in the cache */
static idx_t last_k = 0;         /* last k examined */

/* Ensure the cache can address up to index ‘needed’. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    /* grow approximately twice, but at least ‘needed’ */
    size_t new_cap = cache_cap ? cache_cap * 2 : needed;
    if (new_cap < needed)
        new_cap = needed;

    /* allocate a new array (index 0 unused) */
    seq_t *new_cache = seq_alloc_new_array(new_cap + 1);

    /* copy existing data */
    for (size_t i = 1; i <= count; ++i)
        new_cache[i] = cache[i];

    /* free the old array */
    if (cache != NULL)
        seq_free_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* behaviour below the offset is irrelevant */
        return 0;

    ensure_cache((size_t)n);    /* ensure buffer for index n */

    if ((size_t)n <= count)     /* already computed */
        return cache[n];

    /* Scan new k's until we have collected n terms */
    while (count < n) {
        ++last_k;
        idx_t m2 = last_k << 1;                /* 2 * k   */
        seq_t sigma2k  = sigma_of(m2);
        seq_t sigma2k1 = sigma_of(m2 + 1);     /* 2*k+1   */
        if (sigma2k < sigma2k1) {
            ++count;
            cache[count] = (seq_t)last_k;      /* store k */
        }
    }

    return cache[n];
}
