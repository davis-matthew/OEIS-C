/*
 * A161840  Number of noncentral divisors of n.
 * https://oeis.org/A161840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = tau(n) - 2 + (tau(n) mod 2)
 * where tau(n) is the number of divisors of n.
 * The implementation factors n by trial division and counts divisors.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n at least 1 */
    uint64_t nn = (uint64_t)n;
    uint64_t tmp = nn;
    uint64_t tau = 1;   /* product of (exponent+1) */

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        tau *= 2;
    }

    /* tau - 2 + (tau & 1) */
    int64_t val = (int64_t)tau - 2;
    val += (tau & 1);
    return (seq_t)val;
}
