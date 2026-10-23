/*
 * A047879  a(n)=Sum{T(n,i): i=0,1,...,n}+Sum{T(i,n): i=0,1,...,n-1}, array T
 *          counting least number of knight's moves as in A049604.
 * https://oeis.org/A047879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum{T(n,i): i=0..n} + sum{T(i,n): i=0..n-1} where T(m,n)
 *            is the minimum number of knight moves on an infinite chessboard
 *            from (0,0) to (m,n)
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   knight_dist
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/knight_dist.h"

/* Cached results for a(n). The array is grown lazily for the first index
   requested larger than the current cache. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;

seq_t a(idx_t n)
{
    /* Grow cache if needed */
    if ((size_t)n >= a_cache_size) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();        /* unrecoverable allocation failure */
        a_cache = tmp;

        /* Compute all new entries from old size up to n */
        for (size_t k = a_cache_size; k <= (size_t)n; ++k) {
            if (k == 0) {
                a_cache[k] = (seq_t)0;
                continue;
            }
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i < k; ++i)
                sum += knight_dist((idx_t)k, i);
            seq_t diag = knight_dist((idx_t)k, (idx_t)k);
            a_cache[k] = diag + (seq_t)(2 * sum);
        }
        a_cache_size = new_sz;
    }
    return a_cache[(size_t)n];
}
