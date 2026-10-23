/*
 * A135556  Squares of numbers not divisible by 3: a(n) = A001651(n)^2.
 * https://oeis.org/A135556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2-12*n+4)/4 if n is even; a(n) = (9*n^2-6*n+1)/4 if
 *            n is odd
 * Range:     u8 n <= 5, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n_sq = N * N;
    if ((n & 1u) == 0) /* even */
    {
        seq_t num = n_sq * (seq_t)9 - N * (seq_t)12 + (seq_t)4;
        return num / (seq_t)4;
    }
    else /* odd */
    {
        seq_t num = n_sq * (seq_t)9 - N * (seq_t)6 + (seq_t)1;
        return num / (seq_t)4;
    }
}
