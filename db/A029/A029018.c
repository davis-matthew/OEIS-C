/*
 * A029018  Expansion of 1/((1-x)*(1-x^2)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 54*n^2 + 417*n + 27*n*(-1)^n + 24*n*[(n
 *            mod 3)=0] + 1472)/1296) (nonrecursive, direct computation)
 * Range:     u8 n <= 46, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use signed 64‑bit arithmetic for the intermediate calculation. */
    int64_t nn = (int64_t)n;

    /* Compute the polynomial part: 2*n^3 + 54*n^2 + 417*n */
    int64_t sum = 2 * nn * nn * nn + 54 * nn * nn + 417 * nn;

    /* Add the alternating term 27*n*(-1)^n */
    if ((n & 1ULL) == 0) {
        sum += 27 * nn;          /* even n */
    } else {
        sum -= 27 * nn;          /* odd n   */
    }

    /* Add the indicator term 24*n when n is divisible by 3 */
    if ((n % 3ULL) == 0) {
        sum += 24 * nn;
    }

    /* Add the constant */
    sum += 1472;

    /* Integer division gives the desired floor */
    return (seq_t)(sum / 1296);
}
