/*
 * A022268  a(n) = n*(11*n - 1)/2.
 * https://oeis.org/A022268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n - 1)/2
 * Range:     u8 n <= 6, u16 n <= 109, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn == 0)
        return 0;

    /* compute 11*n (still fits in seq_t for the tested ranges) */
    seq_t mul11 = nn * 11;
    /* 11*n - 1 is non‑negative for n > 0 */
    seq_t numerator = mul11 - 1;

    /* divide first to avoid intermediate overflow */
    if ((nn & 1) == 0)          /* n even: divide n by 2 */
        return (nn / 2) * numerator;
    else            /* n odd: divide numerator by 2 */
        return nn * (numerator / 2);
}
