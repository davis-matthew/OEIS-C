/*
 * A017656  a(n) = (12*n + 11)^4.
 * https://oeis.org/A017656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 20, u64 n <= 5460, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = 12*n + 11 */
    seq_t x = (seq_t)12 * n + (seq_t)11;

    /* Square x once */
    seq_t x2 = x * x;

    /* Square again to get x^4 */
    seq_t result = x2 * x2;

    return result;
}
