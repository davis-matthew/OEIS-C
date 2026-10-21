/*
 * A231675  a(n) = Sum_{i=0..n} digsum_6(i)^4, where digsum_6(i) = A053827(i).
 * https://oeis.org/A231675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_6(i)^4, where digsum_6(i) = sum of
 *            digits of i in base 6
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t s = 0;
        idx_t t = i;
        while (t) {
            s += (seq_t)(t % 6);
            t /= 6;
        }
        seq_t p = s * s * s * s;
        total += p;
    }
    return total;
}
