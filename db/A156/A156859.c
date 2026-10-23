/*
 * A156859  The main column of a version of the square spiral.
 * https://oeis.org/A156859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + n + floor((n+1)/2)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the sequence type */
    seq_t nn = (seq_t)n;

    /* compute n^2 + n */
    seq_t res = nn * nn;
    res += nn;

    /* add floor((n+1)/2) using integer shift */
    res += (nn + 1) >> 1;

    return res;
}
