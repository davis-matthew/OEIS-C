/*
 * A029747  Numbers of the form 2^k times 1, 3 or 5.
 * https://oeis.org/A029747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n < 3) ? n : (n mod 3 + 3) * 2^(floor(n/3)-1)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 93, u64 n <= 189,
 *            u128 n <= 381, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A029747:
   numbers of the form 2^k times 1, 3 or 5. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)          /* a(1)=1, a(2)=2 (and a(0) would be 0 if called) */
        return (seq_t)n;

    idx_t q = n / 3;          /* floor(n/3)   */
    idx_t r = n % 3;          /* n mod 3      */
    seq_t factor = (seq_t)(r + 3);          /* 3,4,5 for r=0,1,2 */
    seq_t pow_val = pow2(q - 1);            /* 2^(q-1) */
    return factor * pow_val;
}
