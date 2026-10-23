/*
 * A043413  Numbers whose base-7 representation contains exactly one 5.
 * https://oeis.org/A043413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑7 representation
 *            contains exactly one digit 5
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_one_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_one_five.h"

/* Cache of already computed terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0; /* number of terms stored (a(1) at cache[0]) */
static size_t cache_cap  = 0; /* allocated capacity of cache */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset is 1 */

    /* Compute missing terms up to n. */
    if (cache_len < (size_t)n) {
        seq_t last = cache_len > 0 ? cache[cache_len - 1] : (seq_t)0;
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t val = last + (seq_t)1;
            while (!has_one_five(val)) val += (seq_t)1;
            ensure_cache_cap(i + 1);
            cache[i] = val;
            last = val;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
