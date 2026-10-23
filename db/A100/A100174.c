/*
 * A100174  Structured tetrakis hexahedral numbers (vertex structure 5).
 * https://oeis.org/A100174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/6)*(20*n^3 - 24*n^2 + 10*n)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 719, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                               /* cast index to seq_t */
    /* compute numerator: n*(20*n^2 - 24*n + 10) */
    seq_t term = nn * (20 * nn * nn - 24 * nn + 10);
    /* divide by 6 – number is always divisible by 6 for integer n */
    term /= 6;
    return term;
}
