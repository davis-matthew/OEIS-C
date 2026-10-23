/*
 * A158907  Row sums of triangle A158906.
 * https://oeis.org/A158907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=1..n} ⌊n/i⌋ - ⌊(n-1)/(i+1)⌋
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n; ++i) {
        seq_t t1 = (seq_t)n / (seq_t)i;
        seq_t t2 = (seq_t)(n - 1) / (seq_t)(i + 1);
        sum += t1 - t2;
    }
    return sum;
}
