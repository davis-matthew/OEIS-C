/*
 * A133648  a(n) = 2*3^n + 3^(n-1) - (n+1).
 * https://oeis.org/A133648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^n + 3^(n-1) - (n+1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* n >= 1 by the offset of the sequence */
    seq_t pow_n  = pow3(n);          /* 3^n   */
    seq_t pow_n1 = pow3(n - 1);      /* 3^(n-1) */
    seq_t res    = ((seq_t)2 * pow_n) + pow_n1 - (seq_t)(n + 1);
    return res;
}
