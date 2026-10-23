/*
 * A048396  Sums of consecutive noncubes.
 * https://oeis.org/A048396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (6*n^5 + 15*n^4 + 18*n^3 + 12*n^2 + 3*n)/2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 66, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */
    seq_t n4 = n3 * n_seq;             /* n^4 */
    seq_t n5 = n4 * n_seq;             /* n^5 */

    seq_t term5 = (seq_t)6 * n5;
    seq_t term4 = (seq_t)15 * n4;
    seq_t term3 = (seq_t)18 * n3;
    seq_t term2 = (seq_t)12 * n2;
    seq_t term1 = (seq_t)3 * n_seq;

    seq_t numerator = term5 + term4 + term3 + term2 + term1;
    return numerator / (seq_t)2;
}
