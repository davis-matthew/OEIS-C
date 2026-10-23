/*
 * A156841  a(n) = 529n^2 - 312n + 46.
 * https://oeis.org/A156841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 529*n^2 - 312*n + 46
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 2849, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the quadratic using seq_t arithmetic to avoid overflow in intermediate
       steps for the supported seq_t sizes. */
    return (seq_t)529 * nn * nn - (seq_t)312 * nn + 46;
}
