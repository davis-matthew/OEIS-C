/*
 * A132127  a(n) = (n^3 + 3*n - 2)/2.
 * https://oeis.org/A132127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 3*n - 2)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the output type before any arithmetic */
    seq_t nn = (seq_t)n;

    /* compute n^2 and n^3 using type seq_t to avoid premature overflow */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* n^3 + 3*n - 2 is always even, so division by 2 yields an integer */
    seq_t result = n3 + 3 * nn - 2;

    return result / 2;
}
