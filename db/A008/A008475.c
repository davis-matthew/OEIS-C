/*
 * A008475  If n = Product (p_j^k_j) then a(n) = Sum (p_j^k_j) (a(1) = 0 by
 *          convention).
 * https://oeis.org/A008475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum of prime powers in the factorization of n. For n = ∏
 *            p_j^k_j, a(n) = Σ p_j^k_j, a(1) = 0
 * Range:     u8 n <= 255, u16 n <= 8413, u32 n <= 8413, u64 n <= 8413,
 *            u128 n <= 8413, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t temp = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* Handle factor 2 */
    if ((temp & 1ULL) == 0) {
        unsigned int cnt = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++cnt;
        }
        seq_t power = (seq_t)1;
        for (unsigned int i = 0; i < cnt; ++i)
            power *= (seq_t)2;
        sum += power;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            seq_t power = (seq_t)1;
            for (unsigned int i = 0; i < cnt; ++i)
                power *= (seq_t)p;
            sum += power;
        }
    }

    /* Remaining prime factor > sqrt(original temp) */
    if (temp > 1) {
        seq_t power = (seq_t)temp; /* temp itself is a prime power of exponent 1 */
        sum += power;
    }

    return sum;
}
