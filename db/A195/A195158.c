/*
 * A195158  Concentric 24-gonal numbers.
 * https://oeis.org/A195158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 5*((-1)^n - 1)/2 (OEIS A195158)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;            /* cast index to sequence type            */
    seq_t val = 6 * k * k;         /* 6 * n * n                              */
    if (n & 1)                     /* subtract 5 only when n is odd           */
        val -= (seq_t)5;
    return val;
}
