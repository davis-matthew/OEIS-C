/*
 * A160938  a(n)= n * digital sum(n+1).
 * https://oeis.org/A160938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * digit_sum(n+1)
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t np = n + 1;
    seq_t sum = 0;
    while (np > 0) {
        sum += (seq_t)(np % 10);
        np /= 10;
    }
    return (seq_t)n * sum;
}
