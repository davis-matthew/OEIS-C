/*
 * A153237  a(n) = A000079(n) - A153130(n).
 * https://oeis.org/A153237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - f(n mod 6) where f = {1, 2, 4, 8, 7, 5} (for
 *            n=0,1,2,3,4,5 respectively)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t offs[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                  (seq_t)8, (seq_t)7, (seq_t)5 };
    seq_t two = pow2(n);                   /* 2^n  */
    return two - offs[n % 6];              /* a(n) = 2^n - offs[n%6] */
}
