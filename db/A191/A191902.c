/*
 * A191902  Number of compositions of odd positive integers into 5 parts <= n.
 * https://oeis.org/A191902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^5 - (1 + (-1)^n)/2)/2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 96, u64 n <= 8190,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n+1) as a 128‑bit value */
    __uint128_t xn = (__uint128_t)n + 1;

    /* compute (n+1)^5 using 128‐bit arithmetic to avoid overflow */
    __uint128_t x2 = xn * xn;     /* (n+1)^2 */
    __uint128_t x3 = x2 * xn;     /* (n+1)^3 */
    __uint128_t x4 = x3 * xn;     /* (n+1)^4 */
    __uint128_t x5 = x4 * xn;     /* (n+1)^5 */

    /* (1 + (-1)^n)/2 is 1 for even n, 0 for odd n */
    __uint128_t small = ((n & 1) == 0) ? 1 : 0;

    /* final result is (x5 - small)/2 */
    __uint128_t res128 = (x5 - small) / 2;

    return (seq_t)res128;
}
