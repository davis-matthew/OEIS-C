/*
 * A094375  a(n) = (4^n - 2^n)/2 + 3^n.
 * https://oeis.org/A094375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n - 2^n)/2 + 3^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_seq_t_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t_int.h"
/* Uses fast exponentiation of small bases.  Works
   correctly for any build that supports seq_t arithmetic. */

seq_t a(idx_t n)
{
    /* Compute 2^n, 3^n and 4^n (= (2^n)^2) */
    seq_t pow2 = pow_seq_t_int(2, n);
    seq_t pow3 = pow_seq_t_int(3, n);
    seq_t pow4 = pow2 * pow2;      /* 4^n = (2^n)^2 */

    /* Compute (4^n - 2^n)/2 + 3^n */
    return (pow4 - pow2) / 2 + pow3;
}
