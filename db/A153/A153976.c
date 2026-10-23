/*
 * A153976  a(n) = n^3 + (n+2)^3.
 * https://oeis.org/A153976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n+2)^3
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 765, u64 n <= 765,
 *            u128 n <= 765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;     /* cast index to seq_t */
    seq_t tn2 = tn + 2;       /* (n + 2) as seq_t */

    seq_t n3  = tn * tn * tn; /* n^3   */
    seq_t n2_3 = tn2 * tn2 * tn2; /* (n+2)^3 */

    return n3 + n2_3;        /* a(n) */
}
