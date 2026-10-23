/*
 * A011872  [ n(n-1)/19 ].
 * https://oeis.org/A011872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n*(n-1)/19]
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute floor(n*(n-1)/19) using integer arithmetic */
    return (nn * (nn - 1)) / (seq_t)19;
}
