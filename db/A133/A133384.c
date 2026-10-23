/*
 * A133384  Numbers with n 0's between 1 and 2.
 * https://oeis.org/A133384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(n+1) + 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A133384. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t result = pow10(n + 1);
    result += (seq_t)2;
    return result;
}
