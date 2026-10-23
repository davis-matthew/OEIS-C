/*
 * A191495  Number of compositions of even natural numbers into 8 parts <= n.
 * https://oeis.org/A191495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)^8 + (1 + (-1)^n)/2)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 278, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate power calculation.
       The final result fits in seq_t whenever it fits in a 64‑bit
       unsigned integer, and the division by 2 is exact for all n. */
    __uint128_t x = (__uint128_t)n + 1;   /* n+1 */
    __uint128_t y = x * x;                /* (n+1)^2  */
    y = y * y;                            /* (n+1)^4  */
    y = y * y;                            /* (n+1)^8  */

    /* Add the parity term: +1 for even n, +0 for odd n.  This guarantees
       that y is even, so the division by 2 is exact. */
    if ((n & 1) == 0) {
        y += 1;
    }
    y >>= 1;  /* divide by 2 */

    return (seq_t)y;
}
