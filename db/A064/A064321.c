/*
 * A064321  a(n) = n*(n-1)^3*(n-2)^3*(n-3).
 * https://oeis.org/A064321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^3*(n-2)^3*(n-3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 17, u64 n <= 257, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the expression contains a factor zero. */
    if (n < 4)
        return (seq_t)0;

    /* Convert idx_t parts to seq_t for correct type operations. */
    seq_t tn  = (seq_t)n;
    seq_t n1  = (seq_t)(n - 1);
    seq_t n2  = (seq_t)(n - 2);
    seq_t n3  = (seq_t)(n - 3);

    /* Compute (n-1)^3 and (n-2)^3 once each. */
    seq_t n1_sq = n1 * n1;
    seq_t n1_c  = n1_sq * n1;

    seq_t n2_sq = n2 * n2;
    seq_t n2_c  = n2_sq * n2;

    /* Final product */
    return tn * n1_c * n2_c * n3;
}
