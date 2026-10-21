/*
 * A091979  Number of odd proper distinct prime divisors of n. That is, the
 *          number of odd distinct prime divisors of n that are less than n.
 * https://oeis.org/A091979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct odd prime divisors of n that are < n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0u;
    idx_t orig = n;
    seq_t count = 0u;

    /* remove factor 2 – it is even and not counted */
    while ((n & 1u) == 0)
        n >>= 1;

    /* trial division by odd numbers */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            count++;
            while (n % p == 0)
                n /= p;
        }
    }

    /* any remaining prime factor > sqrt(orig) */
    if (n > 1 && n != orig)
        count++;

    return count;
}
