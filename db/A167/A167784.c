/*
 * A167784  a(n) = 2^n - (1 - (-1)^n)*3^((n-1)/2).
 * https://oeis.org/A167784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - (1 - (-1)^n)*3^((n-1)/2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2_n = pow2(n);

    /* Even n: a(n) = 2^n */
    if ((n & 1ULL) == 0)
        return pow2_n;

    /* Odd n: a(n) = 2^n - 2*3^((n-1)/2) */
    seq_t pow3_half = pow3((n - 1) / 2);
    return pow2_n - 2 * pow3_half;
}
