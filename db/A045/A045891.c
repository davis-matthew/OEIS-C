/*
 * A045891  First differences of A045623.
 * https://oeis.org/A045891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (n+4) * 2^(n-3) for n ≥ 2, a(0) = a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n < 2)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    /* n ≥ 3: use the closed‑form expression */
    seq_t mult = (seq_t)n;
    mult += (seq_t)4;          /* (n+4) as seq_t */
    seq_t pw  = pow2(n - 3);   /* 2^(n-3) as seq_t */
    return mult * pw;
}
