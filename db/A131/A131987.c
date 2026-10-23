/*
 * A131987  Representation of a dense para-sequence.
 * https://oeis.org/A131987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the label of the n‑th dyadic rational in increasing
 *            order, obtainable by iteratively interleaving the new labels
 *            between the previous segment's labels
 * Range:     u8 n <= 502, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t seg_value(idx_t pos, idx_t seg_k)
{
    if (seg_k == 0)
        return 1;                     /* base segment contains only '1' */
    if (pos & 1)                      /* odd position – a new label          */
    {
        seq_t base   = pow2(seg_k);                      /* 2^seg_k          */
        seq_t offset = (seq_t)((pos - 1) / 2);          /* (pos-1)/2         */
        return base + offset;
    }
    /* even position – value comes from the previous segment */
    return seg_value(pos >> 1, seg_k - 1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* value undefined for indices below offset */

    /* Find the segment k where the n-th term lies.
       Segment k has length L_k = 2^{k+1} - 1.                        */
    idx_t cum = 1;             /* cumulative length up to current segment   */
    idx_t pow = 2;             /* pow = 2^{k+1} for current segment k       */
    idx_t k   = 0;

    while (cum < n)
    {
        pow <<= 1;            /* move to next power of two (2^{k+2})     */
        ++k;                  /* advance segment index                   */
        cum += (pow - 1);     /* add length of the new segment           */
    }

    idx_t prevCum = cum - (pow - 1);    /* length up to previous segment   */
    idx_t pos     = n - prevCum;        /* 1‑based position inside segment */

    return seg_value(pos, k);
}
