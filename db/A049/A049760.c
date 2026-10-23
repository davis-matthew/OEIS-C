/*
 * A049760  a(n) = Sum_{k=1..n} T(n,k), array T as in A049759.
 * https://oeis.org/A049760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (n^2 mod i)
 * Range:     u8 n <= 15, u16 n <= 413, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t sum  = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += n_sq % (seq_t)i;
    return sum;
}
