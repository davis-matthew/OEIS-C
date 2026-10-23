/*
 * A116969  If n mod 2 = 0 then 3*2^(n-1)+n-1 else 3*2^(n-1)+n.
 * https://oeis.org/A116969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) + (n-1 if n even else n) (Offset 1: a(1) = 4,
 *            a(2) = 7, …) The implementation uses the provided pow2 utility
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 3 * 2^(n-1) */
    seq_t val = pow2(n - 1);
    val *= (seq_t)3;

    /* Add n-1 for even n, else add n */
    if ((n & 1) == 0)
        val += (seq_t)n - 1;
    else
        val += (seq_t)n;

    return val;
}
