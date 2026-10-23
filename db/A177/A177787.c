/*
 * A177787  Number of paths from (0,0) to (n+2,n) using only up and right
 *          steps and avoiding two or more consecutive moves up or three or
 *          more consecutive moves right.
 * https://oeis.org/A177787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 11)/6
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The calculation is exact because n*(n^2 + 11) is always divisible by 6. */
    seq_t nn = (seq_t)n;
    return nn * (nn * nn + 11) / 6;
}
