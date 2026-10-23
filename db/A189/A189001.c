/*
 * A189001  a(n) = Sum_{i=0..n} (i+1)*n^i.
 * https://oeis.org/A189001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (i+1)*n^i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;
    seq_t base = (seq_t)n;
    for (idx_t i = 0; i <= n; ++i) {
        sum += (seq_t)(i + 1) * pow;
        pow *= base;
    }
    return sum;
}
