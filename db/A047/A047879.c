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
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Knight distance on an infinite board, adapted from
   https://www.chessprogramming.org/Knight_Distance.
   Small special cases are handled explicitly; all other cases use the
   closed‑form algorithm. */
static seq_t knight_dist(idx_t m, idx_t n)
{
    /* Ensure m >= n for symmetry */
    idx_t dx = (m > n) ? m : n;
    idx_t dy = (m > n) ? n : m;

    /* Basic small‑case corrections */
    if (dx == 0 && dy == 0)        return (seq_t)0;
    if (dx == 1 && dy == 0)        return (seq_t)3;
    if (dx == 2 && dy == 2)        return (seq_t)4;
    if (dx == 1 && dy == 1)        return (seq_t)4;  /* corrected */
    /* The remaining formula works for all other (dx,dy) */
    idx_t d1 = (dx + 1) / 2;           /* floor((dx+1)/2) */
    idx_t d2 = (dx + dy + 2) / 3;      /* floor((dx+dy+2)/3) */
    idx_t d  = (d1 > d2) ? d1 : d2;
    /* Adjust parity */
    if ( (d & 1) != ((dx + dy) & 1) )
        d++;
    return (seq_t)d;
}

/* Cached results for a(n). The array is grown lazily for the first index
   requested larger than the current cache. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;

seq_t a(idx_t n)
{
    /* Grow cache if needed */
    if ((size_t)n >= a_cache_size) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, a_cache, new_sz);
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
