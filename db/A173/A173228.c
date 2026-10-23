/*
 * A173228  The number of trailing zeros in (10^n)!
 * https://oeis.org/A173228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k>=1} floor(10^n / 5^k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 2^n and 5^n as big integers */
    seq_t two_n = pow_uint(2, n);
    seq_t five_n = pow_uint(5, n);

    /* Part contributed by k <= n */
    seq_t part1 = two_n * (five_n - 1) / 4;

    /* Compute floor(log_5(2^n)) = the largest j such that 5^j <= 2^n */
    idx_t e = 0;
    seq_t tmp = two_n;
    while (tmp >= 5) {
        tmp /= 5;
        e++;
    }

    /* Sum for k = n+1 .. n+e  (i.e. j = 1 .. e) of floor(2^n / 5^j) */
    seq_t part2 = 0;
    seq_t pow5 = 5;
    for (idx_t j = 1; j <= e; ++j) {
        part2 += two_n / pow5;
        pow5 *= 5;
    }

    return part1 + part2;
}
