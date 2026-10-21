/*
 * A087893  Number of numbers m satisfying 1 < m < n such that m^2 == m (mod
 *          n).
 * https://oeis.org/A087893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ω(n) - 2 (for n > 1), a(1) = 0 where ω(n) is the number
 *            of distinct prime factors of n
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc. */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n <= 1)
        return (seq_t)0;

    /* Count distinct prime factors of n */
    uint64_t m = (uint64_t)n;
    int omega = 0;

    /* Trial division up to sqrt(m) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            ++omega;
            while (m % p == 0)           /* skip powers of p */
                m /= p;
        }
    }
    /* If anything remains, it is a prime factor > sqrt(original n) */
    if (m > 1)
        ++omega;

    /* Compute 2^omega - 2, safe for the expected size of n */
    seq_t val = (seq_t)(((uint64_t)1 << omega) - 2);
    return val;
}
