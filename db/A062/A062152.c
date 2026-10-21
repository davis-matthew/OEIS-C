/*
 * A062152  Sixth (unsigned) column of triangle A062138 (generalized a=5
 *          Laguerre).
 * https://oeis.org/A062152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)! * binomial(n+10,10) / 5!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t types used by the build */

seq_t a(idx_t n)
{
    /* Denominator: 10! = 2^8 * 3^4 * 5^2 * 7^1 */
    int exp2 = 8, exp3 = 4, exp5 = 2, exp7 = 1;

    seq_t res = 1;

    /* Multiply the factors from (n+5)!/5!  -> product of 6 .. n+5   */
    for (idx_t k = 6; k <= n + 5; ++k) {
        res *= (seq_t)k;

        /* cancel any remaining small prime factors from 10!   */
        while (exp2 > 0 && (res % 2) == 0) { res /= 2; --exp2; }
        while (exp3 > 0 && (res % 3) == 0) { res /= 3; --exp3; }
        while (exp5 > 0 && (res % 5) == 0) { res /= 5; --exp5; }
        while (exp7 > 0 && (res % 7) == 0) { res /= 7; --exp7; }
    }

    /* Multiply the binomial numerator (n+1)..(n+10) and cancel the 10!   */
    for (idx_t k = 1; k <= 10; ++k) {
        seq_t mult = (seq_t)(n + k);
        res *= mult;

        while (exp2 > 0 && (res % 2) == 0) { res /= 2; --exp2; }
        while (exp3 > 0 && (res % 3) == 0) { res /= 3; --exp3; }
        while (exp5 > 0 && (res % 5) == 0) { res /= 5; --exp5; }
        while (exp7 > 0 && (res % 7) == 0) { res /= 7; --exp7; }
    }

    /* All denominator factors should have been cancelled */
    return res;
}
