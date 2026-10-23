/*
 * A014816  a(n) = Sum_{k=1..n} ceiling(k^4/n).
 * https://oeis.org/A014816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ceil(k^4 / n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 288, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t kk2 = kk * kk;
        seq_t kk4 = kk2 * kk2;
        seq_t val = (kk4 + sn - 1) / sn;  /* ceil division */
        sum += val;
    }
    return sum;
}
