/*
 * A100155  Structured truncated octahedral numbers.
 * https://oeis.org/A100155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(17*n^2 - 18*n + 4)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 911, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;         /* convert index to seq_t */
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t inner = (seq_t)17 * n2 - (seq_t)18 * tn + (seq_t)4;
    seq_t num   = tn * inner;    /* n * inner */
    seq_t ans   = num / (seq_t)3;
    return ans;
}
