/*
 * A038713  a(n) = n XOR (n-1), i.e., nim-sum of sequential pairs, written in
 *          binary.
 * https://oeis.org/A038713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = (10^(tz(n)+1) - 1)/9, where tz(n) is the number of
 *            trailing * zeros in the binary representation of n.
 *            Equivalently, a(n) is the decimal * number consisting of tz(n)+1
 *            digits each equal to 1. * This computes the nim-sum n XOR (n-1)
 *            represented as a decimal integer
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

static idx_t trailing_zeros(idx_t x)
{
    idx_t cnt = 0;
    while ((x & 1U) == 0ULL) {
        ++cnt;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the sequence definition */
    idx_t tz = trailing_zeros(n);   /* count of trailing zeros in n */
    idx_t k  = tz + 1;              /* number of consecutive 1-digits */
    seq_t val = pow10(k);           /* 10^k in arbitrary precision (or native) */
    val -= 1;                       /* 10^k - 1   */
    val /= 9;                       /* (10^k - 1) / 9  -> number with k '1' digits */
    return val;
}
