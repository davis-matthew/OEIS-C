/*
 * A087863  a(n) = (n^3 + 24*n^2 + 65*n + 36)/6.
 * https://oeis.org/A087863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 24*n^2 + 65*n + 36) / 6
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 2673, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the sequence type to avoid unintended implicit casts */
    seq_t nn = (seq_t)n;

    /* compute the polynomial exactly in seq_t arithmetic */
    seq_t val = nn * nn * nn       /* n^3 */
             + (seq_t)24 * nn * nn /* 24*n^2 */
             + (seq_t)65 * nn      /* 65*n */
             + (seq_t)36;          /* constant term */

    /* divide by 6 – the result is always an integer */
    val /= (seq_t)6;
    return val;
}
