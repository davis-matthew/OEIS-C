/*
 * A014819  a(n) = Sum_{k=1..n} floor(k^4/n).
 * https://oeis.org/A014819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(k^4 / n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 381, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    seq_t div = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ks = (seq_t)k;
        seq_t k2 = ks * ks;
        seq_t k4 = k2 * k2;
        sum += k4 / div;
    }

    return sum;
}
