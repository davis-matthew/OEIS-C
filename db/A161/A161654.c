/*
 * A161654  a(n) = the largest multiple of {the number of divisors of n} that
 *          is <= n.
 * https://oeis.org/A161654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / τ(n)) * τ(n) where τ(n) is the number of
 *            positive divisors of n
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* tau(n) – number of divisors of n */
    uint64_t m = (uint64_t)n;
    uint64_t temp = m;
    uint64_t tau = 1;

    /* factor 2 separately */
    uint64_t c = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++c;
    }
    if (c) tau *= (c + 1);

    /* odd factors */
    for (uint64_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            c = 0;
            while (temp % i == 0) {
                temp /= i;
                ++c;
            }
            tau *= (c + 1);
        }
    }

    if (temp > 1) tau *= 2;   /* remaining prime > sqrt(m) */

    /* compute a(n) = (m / tau) * tau */
    seq_t d = (seq_t)tau;
    seq_t q = ((seq_t)m / d) * d;
    return q;
}
