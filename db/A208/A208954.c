/*
 * A208954  a(n) = n^4*(n-1)*(n+1)/12.
 * https://oeis.org/A208954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n-1)*(n+1)/12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast index to seq_t to avoid accidental overflow of the
       intermediate calculations.  Intermediate results are
       expressed solely with seq_t arithmetic to keep the code
       valid for all build configurations, regardless of whether
       seq_t is 8‑, 16‑, 32‑, 64‑, 128‑bit or an arbitrary‑precision
       integer class. */
    seq_t nn = (seq_t)n;            /* n as seq_t                */
    seq_t n2 = nn * nn;             /* n^2                        */
    seq_t n4 = n2 * n2;             /* n^4                        */
    seq_t num = n4 * (nn - 1) * (nn + 1); /* n^4*(n-1)*(n+1)   */
    return num / 12;                /* divide by 12, exact        */
}
