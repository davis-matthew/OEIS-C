/*
 * A152581  Generalized Fermat numbers: a(n) = 8^(2^n) + 1, n >= 0.
 * https://oeis.org/A152581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 9 Recurrence: a(n) = (a(n-1) - 1)^2 + 1 for n ≥ 1 The
 *            implementation iterates from the base case, using only
 *            arithmetic operators valid for both integral and
 *            arbitrary‑precision types
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)9;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i)         /* produce a(1)…a(n) */
    {
        seq_t t   = val - (seq_t)1;       /* a(k-1) - 1      */
        val = t * t + (seq_t)1;          /* (a(k-1)-1)^2 + 1*/
    }
    return val;                           /* a(n) */
}
