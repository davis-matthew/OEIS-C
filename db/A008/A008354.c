/*
 * A008354  a(n) = (5*n^2 + 1)*n^2 / 6.
 * https://oeis.org/A008354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^2 + 1)*n^2 / 6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 171, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index first, to avoid signed/unsigned mix issues */
    seq_t nn = (seq_t)n;
    /* Compute n^2 */
    seq_t n2 = nn * nn;
    /* Evaluate the formula, keeping the intermediate value within seq_t */
    seq_t res = (5 * n2 + 1) * n2 / 6;
    return res;
}
