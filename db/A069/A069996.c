/*
 * A069996  Number of spanning trees on the bipartite graph K_{3,n}.
 * https://oeis.org/A069996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * 3^(n-1) (offset 0, a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t pow3 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow3 *= (seq_t)3;
    }

    seq_t nsq = (seq_t)n * (seq_t)n;
    return pow3 * nsq;
}
