/*
 * A132297  Number of distinct Markov type classes of order 2 possible in
 *          binary strings of length n.
 * https://oeis.org/A132297
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (15 + (-1)^n - 4*n + 6*n^2)/8
 * Range:     u8 n <= 18, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n < 2 return 0 (offset behaviour is undefined) */
    if (n < 2)
        return 0;

    idx_t nn = n;
    /* compute numerator: 6*n^2 - 4*n + (n even ? 16 : 14) */
    idx_t num = (idx_t)6 * nn * nn - (idx_t)4 * nn
                + ((nn & 1) ? (idx_t)14 : (idx_t)16);

    return (seq_t)(num / 8);
}
