/*
 * A172404  Numbers k such that 3 is the first digit of 2^k.
 * https://oeis.org/A172404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 8188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth exponent k such that 2^k starts with the digit 3
 * (OEIS A172404). 
 * The function iterates k, checking 2^k's leading digit by using the
 * logarithm base 10.  The leading digit is 3 iff the fractional part
 * of k·log10(2) lies in [log10(3), log10(4)).  This test is
 * implemented with double precision; the density is about 0.125,
 * so for n ≤ 10000 we examine <  {n/0.125 ≈ 80 000} values of k,
 * well within the time limit. */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* constant log10(2) with high precision */
    const double LOG10_2 = 0.3010299956639811952137388947244930;
    const double LOG10_3 = 0.47712125471966244;  /* log10(3)   */
    const double LOG10_4 = 0.60205999132796239;  /* log10(4)   */

    idx_t k = 1;
    idx_t count = 0;

    while (true) {
        double x = k * LOG10_2;
        double fractional = x - floor(x);

        /* 10^fractional gives the first digits of 2^k as a double */
        double first = pow(10.0, fractional);

        /* Check whether it falls in the interval [3,4) */
        if (first >= 3.0 && first < 4.0) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
