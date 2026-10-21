/*
 * A053815  Floor(n / (sum of proper divisors of n)).
 * https://oeis.org/A053815
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / (sum of proper divisors of n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sum of all divisors sigma(n) */
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            sigma += (seq_t)i;
            if (j != i)
                sigma += (seq_t)j;
        }
    }
    seq_t sum_proper = sigma - (seq_t)n;
    if (sum_proper == 0)
        return 0;
    return (seq_t)n / sum_proper;
}
