/*
 * A146076  Sum of even divisors of n.
 * https://oeis.org/A146076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for odd n, a(n) = 2 * σ(n/2) for even n
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ(m) is the sum of all positive divisors of m */

#include <stdint.h>

/* Helper: compute σ(n) for n >= 1 */
static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    /* handle factor 2 */
    if ((temp & 1ULL) == 0) {
        seq_t sum = 1;
        seq_t power = 1;
        while ((temp & 1ULL) == 0) {
            power *= 2;
            sum += power;
            temp >>= 1;
        }
        result *= sum;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t sum = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                sum += power;
            }
            result *= sum;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        result *= (seq_t)(1 + (seq_t)temp);
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if ((n & 1ULL) != 0) {
        return (seq_t)0;
    }
    idx_t k = n >> 1;                  /* k = n/2 */
    seq_t sigma_k = sigma_of(k);       /* σ(k) */
    return sigma_k * (seq_t)2;         /* 2 * σ(k) */
}
