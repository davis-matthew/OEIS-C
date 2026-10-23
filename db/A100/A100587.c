/*
 * A100587  Number of nonempty subsets of divisors of n.
 * https://oeis.org/A100587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1 + 2^tau(n), tau(n) = number of divisors of n
 * Range:     u8 n <= 35, u16 n <= 179, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute tau(n) */
    seq_t tau = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)        /* remaining prime factor */
        tau *= (seq_t)2;

    /* Compute 2^tau - 1 */
    seq_t result = 1;
    idx_t t = (idx_t)tau;   /* tau will never exceed UINT64_MAX for n up to 10000 */
    for (idx_t i = 0; i < t; ++i)
        result *= (seq_t)2;
    result -= (seq_t)1;
    return result;
}
