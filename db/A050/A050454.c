/*
 * A050454  a(n) = Sum_{d|n, d=3 mod 4} d^3.
 * https://oeis.org/A050454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d≡3 (mod 4)} d³
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i % 4 == 3) {
                seq_t di = (seq_t)i;
                sum += di * di * di;
            }
            idx_t j = n / i;
            if (j != i && j % 4 == 3) {
                seq_t dj = (seq_t)j;
                sum += dj * dj * dj;
            }
        }
    }
    return sum;
}
