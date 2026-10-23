/*
 * A030140  The nonsquares squared.
 * https://oeis.org/A030140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000037(n)^2 - square of the nth nonsquare integer
 * Range:     u8 n <= 12, u16 n <= 240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the nth nonsquare integer f.  The count of nonsquares <= x is
       x - ⌊√x⌋.  We search for the smallest f such that f - ⌊√f⌋ == n. */
    seq_t low  = (seq_t)n;                 /* f cannot be smaller than n */
    seq_t high = low + floor_sqrt(low) + 10; /* generous upper bound */
    seq_t n_seq = (seq_t)n;

    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t s   = floor_sqrt(mid);       /* ⌊√mid⌋ */
        seq_t diff = mid - s;              /* count of nonsquares <= mid */

        if (diff >= n_seq)
            high = mid;
        else
            low = mid + 1;
    }

    /* Now low is the nth nonsquare integer; return its square. */
    return low * low;
}
