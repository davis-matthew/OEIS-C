/*
 * A154569  Partial sums of (2n-1)^(2n+1)+(2n+1)^(2n-1).
 * https://oeis.org/A154569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+(2n-1)^(2n+1)+(2n+1)^(2n-1); a(0)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static size_t  cache_len = 0;
    static idx_t   last_computed = 0;

    /* initialise cache for the base case a(0)=0 */
    if (cache == NULL) {
        cache = seq_alloc_array(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
        last_computed = 0;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = seq_alloc_array(new_len);
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* value already computed */
    if (n <= last_computed) {
        return cache[n];
    }

    /* compute new terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term1 = ipow_idx((seq_t)(2 * i - 1), 2 * i + 1);
        seq_t term2 = ipow_idx((seq_t)(2 * i + 1), 2 * i - 1);
        cache[i] = cache[i - 1] + term1 + term2;
    }

    last_computed = n;
    return cache[n];
}
