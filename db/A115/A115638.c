/*
 * A115638  A Jacobsthal-related divide-and-conquer sequence.
 * https://oeis.org/A115638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1; a(1)=-1; a(2n+1) = -1; a(2n) = 4*a(n-1)+1 (n≥1)
 * Range:     i8 n <= 29, i16 n <= 509, i32 n <= 1024, i64 n <= 1024,
 *            i128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return -(seq_t)1;
    if (n & 1) return -(seq_t)1;          /* odd indices */
    /* even indices: n = 2*n2, n2≥1 */
    return ((seq_t)4) * a(n / 2 - 1) + 1;
}
