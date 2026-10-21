/*
 * A050453  Sum_{d|n, d=3 mod 4} d^2.
 * https://oeis.org/A050453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d≡3 (mod 4)} d^2
 * Range:     u8 n <= 18, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 according to the specification */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if ((n % i) == 0) {
            idx_t j = n / i;
            if ((i & 3) == 3) {               /* i ≡ 3 (mod 4) */
                seq_t di = (seq_t)i;
                sum += di * di;
            }
            if (j != i && (j & 3) == 3) {     /* j ≡ 3 (mod 4) and j != i */
                seq_t dj = (seq_t)j;
                sum += dj * dj;
            }
        }
    }
    return sum;
}
