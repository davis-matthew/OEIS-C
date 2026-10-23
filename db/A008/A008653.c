/*
 * A008653  Theta series of direct sum of 2 copies of hexagonal lattice.
 * https://oeis.org/A008653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 12*σ(n) - 36*σ(n/3) where σ(n) is
 *            the sum of positive divisors of n
 * Range:     u8 n <= 13, u16 n <= 2199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum of positive divisors of m (m > 0). */
static seq_t sigma_uint64(idx_t m) {
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(m) avoiding overflow of i*i */
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(m / i);
            sum += d1;
            if (d2 != d1) sum += d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t s = sigma_uint64(n);
    if (n % 3 == 0) {
        s -= 3UL * sigma_uint64(n / 3);
    }
    return 12 * s;
}
