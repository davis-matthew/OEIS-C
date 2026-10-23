/*
 * A128959  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by at least one of the
 *          primes 2,3 and is not divisible by at least one of the primes 5,7.
 * https://oeis.org/A128959
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - floor(10^n/6) - floor(10^n/35) + floor(10^n/210)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t N = pow10(n);
    seq_t res = N - N/6 - N/35 + N/210;
    return res;
}
