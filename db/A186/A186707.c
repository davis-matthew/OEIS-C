/*
 * A186707  Partial sums of A007202 (crystal ball sequence for hexagonal
 *          close-packing).
 * https://oeis.org/A186707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (14*n*(n^3 + 4*n^2 + 6*n + 4) + (15 + (-1)^n)) / 16
 *            (using integer arithmetic; intermediate values fit in seq_t for
 *            n ≤ 10^7)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 262, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                 /* convert index to seq_t */
    seq_t n2 = n_seq * n_seq;               /* n^2  */
    seq_t n3 = n2 * n_seq;                  /* n^3  */
    seq_t sum = n3 + (4 * n2) + (6 * n_seq) + 4;
    seq_t N = (14 * n_seq * sum) + ((n & 1) ? (seq_t)14 : (seq_t)16);
    return N / 16;
}
