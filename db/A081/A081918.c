/*
 * A081918  a(0) = 1; a(n) = n^(n-1)(3n-1)/2 (n>0).
 * https://oeis.org/A081918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n-1) * (3n-1) / 2 (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t pow = (seq_t)1;
    seq_t nval = (seq_t)n;
    for (idx_t i = 1; i < n; ++i) {
        pow *= nval;
    }
    seq_t prod = pow * (seq_t)(3ULL * n - 1);
    return prod / (seq_t)2;
}
