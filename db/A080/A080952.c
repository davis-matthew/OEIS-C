/*
 * A080952  a(n) = 2^(n-4)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)/15.
 * https://oeis.org/A080952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * binomial(n+6,5) (offset 0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 43, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 3 */
        return (seq_t)3;

    /* Compute binomial coefficient C(n+6,5) iteratively to keep
       intermediate values small. */
    seq_t bin = 1;
    idx_t m = n + 6;          /* m = n+6  */
    for (idx_t i = 1; i <= 5; ++i)
        bin = bin * (m - i + 1) / i;   /* divide at each step */

    /* Compute 2^(n-1) by repeated left shift. */
    seq_t pw = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        pw <<= 1;

    return pw * bin;
}
