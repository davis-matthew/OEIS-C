/*
 * A231670  a(n) = Sum_{i=0..n} digsum_5(i)^3, where digsum_5(i) = A053824(i).
 * https://oeis.org/A231670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_5(i)^3 where digsum_5(i) is the sum
 *            of the digits of i in base 5
 * Range:     u8 n <= 8, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t x = i;
        idx_t dsum = 0;
        while (x > 0) {
            dsum += x % 5;
            x /= 5;
        }
        seq_t d = (seq_t)dsum;
        seq_t cube = d * d * d;
        sum += cube;
    }
    return sum;
}
