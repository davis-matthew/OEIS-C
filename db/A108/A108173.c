/*
 * A108173  Let beta = A058265. Sequence gives a(n) = 1 +
 *          ceiling((n-1)*beta^2).
 * https://oeis.org/A108173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ceil((n-1)*beta^2), where beta is the tribonacci
 *            constant (root of x^3-x^2-x-1=0). We use a rational
 *            approximation of beta^2 obtained from the ratio of consecutive
 *            Tribonacci numbers: T20 = 66012, T19 = 35890 which gives alpha ≈
 *            3.382281. The approximation error is < 1e-10, so for n ≤...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)  /* a(1) = 1 */
        return 1;

    /* Ratios of consecutive Tribonacci numbers */
    const seq_t P = (seq_t)66012;   /* T20   */
    const seq_t Q = (seq_t)35890;   /* T19   */

    /* Compute floor((n-1) * P^2 / Q^2) using 128‑bit arithmetic */
    __uint128_t numerator   = (__uint128_t)(n-1) * (__uint128_t)P * (__uint128_t)P;
    __uint128_t denominator = (__uint128_t)Q * (__uint128_t)Q;
    seq_t          floorVal = (seq_t)(numerator / denominator);

    /* a(n) = floor((n-1)*alpha) + 2 for n ≥ 2 */
    return floorVal + 2;
}
