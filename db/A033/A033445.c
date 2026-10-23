/*
 * A033445  a(n) = (n - 1)*(n^2 + n - 1).
 * https://oeis.org/A033445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(n^2 + n - 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 780, u64 n <= 780,
 *            u128 n <= 780, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute n^3 - 2n + 1, which equals (n - 1)*(n^2 + n - 1) */
    seq_t result = nn * nn * nn;
    result -= 2 * nn;
    result += 1;
    return result;
}
