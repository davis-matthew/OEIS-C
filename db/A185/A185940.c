/*
 * A185940  a(n) = 1 - 2^(n+1) + 3^(n+2).
 * https://oeis.org/A185940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - 2^(n+1) + 3^(n+2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* n is 1‑based as per OEIS offset.  For n>=1 the value
       3^(n+2) is always larger than 2^(n+1), so the
       difference is non‑negative. */
    int e2 = (int)(n + 1);   /* exponent for 2, fits in 32‑bit int for all tested n */
    int e3 = (int)(n + 2);   /* exponent for 3 */
    seq_t two_pow   = pow_seq((seq_t)2, e2);
    seq_t three_pow = pow_seq((seq_t)3, e3);
    seq_t diff = three_pow - two_pow;
    return diff + (seq_t)1;
}
