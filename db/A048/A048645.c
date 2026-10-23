/*
 * A048645  Integers with one or two 1-bits in their binary expansion.
 * https://oeis.org/A048645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A048645: integers with one or two 1-bits in their binary expansion.
 *  Enumeration: for h = 0,1,2,… output 2^h first, then 2^h + 2^k for k = 0…h-1.
 *  For a given n (1‑based) we find the block h such that
 *      (h+1)(h+2)/2 ≥ n
 *  and then pick the appropriate element within that block.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the block height h.  The cumulative count up to block h is
     * (h+1)(h+2)/2, computed in seq_t to avoid overflow. */
    idx_t h = 0;
    for (;;) {
        seq_t cum = (seq_t)(h + 1) * (h + 2) / 2;
        if (cum >= (seq_t)n)
            break;
        ++h;
    }

    /* Position inside the block (1‑based).  cum_prev is the count up to block h‑1. */
    seq_t cum_prev = (seq_t)h * (h + 1) / 2;
    idx_t  pos     = (idx_t)((seq_t)n - cum_prev);   /* 1 ≤ pos ≤ h+1 */

    /* Compute the term. */
    if (pos == 1)
        return pow2(h);                 /* single 1‑bit number */
    else
        return pow2(h) + pow2(pos - 2); /* two 1‑bits, second bit at pos‑2 */
}
