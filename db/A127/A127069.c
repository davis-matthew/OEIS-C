/*
 * A127069  Number of lines in a Pauli graph of order n.
 * https://oeis.org/A127069
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + 3*2^n + 4^n)/2 for n>1 Recurrence: a(n) = 7*a(n-1) -
 *            14*a(n-2) + 8*a(n-3) for n>4 Initial values: a(2)=15, a(3)=45,
 *            a(4)=153, a(5)=561
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* behaviour below the offset is irrelevant */

    /* compute 2^n and 4^n using the provided utility functions */
    seq_t two_pow  = pow2(n);
    seq_t four_pow = pow4(n);

    /* a(n) = (2 + 3*2^n + 4^n)/2  */
    seq_t sum = (seq_t)2 + (seq_t)3 * two_pow + four_pow;
    return sum / 2;
}
