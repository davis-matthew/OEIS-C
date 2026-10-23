/*
 * A128954  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by the prime 2 and is
 *          not divisible by at least one of the primes 3, 5 and 7.
 * https://oeis.org/A128954
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - floor(10^n/2) - floor(10^n/105) + floor(10^n/210)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* For values below the offset we return 0; they are out of scope. */
    if (n < 2) {
        return 0;
    }
    seq_t p10 = pow10(n);          /* 10^n */
    seq_t t_half = p10 / 2;        /* floor(10^n / 2) */
    seq_t t_105 = p10 / 105;       /* floor(10^n / 105) */
    seq_t t_210 = p10 / 210;       /* floor(10^n / 210) */
    return p10 - t_half - t_105 + t_210;
}
