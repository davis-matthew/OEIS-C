/*
 * A096959  Sixth column (m=5) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A096959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+30)*(n+4)*(n+3)*(n+2)*(n+1)/120
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 212, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for intermediate values. */
    __uint128_t x = ( __uint128_t )(n + 30);
    x *= (n + 4);
    x *= (n + 3);
    x *= (n + 2);
    x *= (n + 1);
    x /= 120;
    return (seq_t)x;
}
