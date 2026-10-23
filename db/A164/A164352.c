/*
 * A164352  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)-a(n-2)+2*a(n-3)-a(n-4)+2*a(n-5)-a(n-6) (n >= 7,
 *            initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=24,
 *            a(5)=48, a(6)=96)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* static cache for computed values */
static seq_t *seq_cache   = NULL;   /* pointer to allocated array         */
static idx_t  cache_len   = 0;       /* number of terms actually computed  */
static size_t cache_cap   = 0;       /* allocated capacity (in terms)      */

/* Ensure the cache array has capacity for at least `n+1` terms. */
static void grow_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (cache_cap >= (size_t)needed) {
        return;
    }

    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < (size_t)needed) {
        new_cap <<= 1;
    }

    seq_t *new_arr = alloc_seq_array(new_cap);
    if (seq_cache) {
        /* copy previous values */
        for (size_t i = 0; i < cache_len; ++i) {
            new_arr[i] = seq_cache[i];
        }
        seq_free_array(seq_cache);
    }

    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Return a(n) (any order of calls). */
seq_t a(idx_t n)
{
    /* Initialise cache with first 7 terms if necessary. */
    if (!seq_cache) {
        /* Allocate space for at least the first 7 terms. */
        grow_cache(6);
        cache_len = 7;
        seq_cache[0] = 1;
        seq_cache[1] = 3;
        seq_cache[2] = 6;
        seq_cache[3] = 12;
        seq_cache[4] = 24;
        seq_cache[5] = 48;
        seq_cache[6] = 96;
    }

    /* n is within already computed range. */
    if (n < cache_len) {
        return seq_cache[n];
    }

    /* Ensure capacity for the requested index. */
    grow_cache(n);

    /* Compute missing terms up to n (inclusive). */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_cache[i] =
            (2 * seq_cache[i-1] - seq_cache[i-2]) +
            (2 * seq_cache[i-3] - seq_cache[i-4]) +
            (2 * seq_cache[i-5] - seq_cache[i-6]);
    }
    cache_len = n + 1;

    return seq_cache[n];
}
