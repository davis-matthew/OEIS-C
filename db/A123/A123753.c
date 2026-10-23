/*
 * A123753  Partial sums of A070941.
 * https://oeis.org/A123753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003314(n+1)+1 = (n+1) + (n+1)*floor(log2(n+1)+1) -
 *            2^floor(log2(n+1)+1) + 1
 * Range:     u8 n <= 44, u16 n <= 5265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* static helper: floor(log2(v)) for unsigned 64‑bit integer */
static idx_t _floor_log2_u64(idx_t v)
{
    /* v > 0 */
    idx_t msb = 0;
    for (idx_t i = 63; i > 0; --i)
    {
        if (v >> i)
        {
            msb = i;
            break;
        }
    }
    return msb;
}

seq_t a(idx_t n)
{
    /* m = n+1 (in idx_t)                               */
    idx_t m_idx = n + 1;

    /* convert to seq_t for computing the result       */
    seq_t m = (seq_t)m_idx;

    /* compute bil(m) = floor(log2(m)) + 1   (0 for m=0) */
    idx_t bil_idx = 0;
    if (m_idx > 0)
    {
        bil_idx = _floor_log2_u64(m_idx) + 1;
    }

    seq_t bil   = (seq_t)bil_idx;            /* cast to seq_t   */
    seq_t pow2  = ((seq_t)1) << (uint64_t)bil_idx;

    /* a(n) = m + m*bil - pow2 + 1                */
    seq_t result = m + m * bil - pow2 + 1;
    return result;
}
