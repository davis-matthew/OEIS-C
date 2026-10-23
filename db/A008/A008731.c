/*
 * A008731  Molien series for 3-dimensional group [2, n] = *22n.
 * https://oeis.org/A008731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/48)*(2*n^2 + 14*n + 27 + (6*n+21)*(-1)^n - 16*(n=1 mod
 *            3))
 * Range:     u8 n <= 7, u16 n <= 234, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t expr = 2 * nn * nn + 14 * nn + 27;
    seq_t t    = 6 * nn + 21;

    /* Add or subtract t depending on the parity of n */
    if ((n & 1) == 0)
        expr += t;
    else
        expr -= t;

    /* Subtract 16 when n ≡ 1 (mod 3) */
    if (n % 3 == 1)
        expr -= 16;

    return expr / (seq_t)48;
}
