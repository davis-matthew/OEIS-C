/*
 * A153783  3 times 11-gonal (or hendecagonal) numbers: a(n) = 3*n*(9*n-7)/2.
 * https://oeis.org/A153783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*(9*n-7)/2
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t */
    seq_t term = nn * (((seq_t)9 * nn) - (seq_t)7);
    return (term * (seq_t)3) / (seq_t)2;  /* exact integer division */
}
