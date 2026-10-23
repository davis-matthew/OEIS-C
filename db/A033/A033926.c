/*
 * A033926  Base 6 digital convolution sequence.
 * https://oeis.org/A033926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{∞} d_i * a(i), where d_i are the base‑6 digits
 *            of n (least significant digit first). a(0) = 1. The function
 *            uses a lazy expanding cache so that calls can arrive in any
 *            order
 * Range:     u8 n <= 7776, u16 n <= 7776, u32 n <= 7776, u64 n <= 7776,
 *            u128 n <= 7776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, abort */
#include <stdint.h>  /* for uint8_t and size_t */

seq_t a(idx_t n)
{
    /* Lazily expanded cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed entries */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache is initialized */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        cache_len = 1;
    }

    /* Expand cache if necessary to store all values up to index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < n + 1) {
            new_cap <<= 1;  /* double capacity */
        }
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
        seq_t val = (seq_t)0;
        idx_t temp = (idx_t)idx;   /* copy of index to inspect digits */
        size_t pos = 0;            /* digit position */
        while (temp != 0) {
            uint8_t digit = (uint8_t)(temp % 6);
            val += (seq_t)digit * cache[pos];
            temp /= 6;
            ++pos;
        }
        cache[idx] = val;
    }
    cache_len = (size_t)n + 1;  /* all computed up to n */

    return cache[n];
}
