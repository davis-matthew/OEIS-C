/*
 * A098798  a(n) = Sum_{1<k<n and k not dividing n} floor(n/k).
 * https://oeis.org/A098798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1}^{n}⌊n/k⌋ – σ(n) (sum of floors minus sum of
 *            divisors)
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sum_{k=1}^{n} floor(n/k) */
    seq_t sum_floor = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum_floor += (seq_t)(n / k);

    /* σ(n): sum of divisors of n */
    seq_t sigma = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sigma += (seq_t)d;
            if (d != n / d)
                sigma += (seq_t)(n / d);
        }
    }

    return sum_floor - sigma;
}
