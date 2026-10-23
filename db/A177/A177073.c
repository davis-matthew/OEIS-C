/*
 * A177073  a(n) = (9*n+4)*(9*n+5).
 * https://oeis.org/A177073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+4)*(9*n+5)
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t  = (seq_t)9 * tn;
    seq_t term1 = t + (seq_t)4;
    seq_t term2 = t + (seq_t)5;
    return term1 * term2;
}
