/*
 * A190176  a(n) = n^4 + 2^4 + (n+2)^4.
 * https://oeis.org/A190176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 + 2*n + 4)^2
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 214, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute n^2 */
    seq_t n2 = nn * nn;
    /* inside = n^2 + 2*n + 4 */
    seq_t inside = n2 + ((seq_t)2 * nn) + ((seq_t)4);
    /* a(n) = 2 * inside^2 */
    seq_t result = inside * inside;
    result = result * ((seq_t)2);
    return result;
}
