/*
 * A222416  If n = Product (p_j^k_j) then a(n) = Sum (p_j^k_j) (a(1) = 1 by
 *          convention).
 * https://oeis.org/A222416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of prime powers in the factorization of n (n = ∏
 *            p_j^k_j → a(n) = Σ p_j^k_j, a(1) = 1)
 * Range:     u8 n <= 255, u16 n <= 9965, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 by convention */
    if (n == 1) return (seq_t)1;

    seq_t sum = 0;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = 0;
        /* Find smallest prime divisor of m */
        if ((m & 1ULL) == 0) {
            p = 2;
        } else {
            for (uint64_t d = 3; d <= m / d; d += 2) {
                if (m % d == 0) {
                    p = d;
                    break;
                }
            }
            if (p == 0) p = m; /* m itself is prime */
        }

        /* Count exponent k of p in m */
        uint64_t k = 0;
        while (m % p == 0) {
            m /= p;
            ++k;
        }

        /* Compute p^k as seq_t */
        seq_t power = 1;
        for (uint64_t i = 0; i < k; ++i) {
            power *= (seq_t)p;
        }

        sum += power;
    }

    return sum;
}
