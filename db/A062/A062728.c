/*
 * A062728  Second 11-gonal (or hendecagonal) numbers: a(n) = n*(9*n+7)/2.
 * https://oeis.org/A062728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n+7)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = nn * 9 + 7;   /* compute 9*n + 7 */
    return nn * temp / 2;      /* compute n*(9*n+7)/2 */
}
