/*
 * A231677  a(n) = Sum_{i=0..n} digsum_7(i)^2, where digsum_7(i) = A053828(i).
 * https://oeis.org/A231677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_7(i)^2
 * Range:     u8 n <= 15, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t v = i;
        unsigned d = 0;
        while (v) {
            d += (unsigned)(v % 7);
            v /= 7;
        }
        sum += (seq_t)d * (seq_t)d;
    }
    return sum;
}
