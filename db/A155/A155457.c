/*
 * A155457  a(n) = exp(Lambda(n)), where Lambda(n) is the von Mangoldt
 *          function for odd (!) primes.
 * https://oeis.org/A155457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p if n = p^k where p is an odd prime and k >= 1,
 *            otherwise 1
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial n explicitly. */
    if (n <= 2)
        return (seq_t)1;

    /* Even n cannot be a power of an odd prime. */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* Find the smallest odd prime divisor of n. */
    idx_t p = 0;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) {
            p = i;
            break;
        }
    }

    /* If no odd divisor was found, n itself is an odd prime. */
    if (p == 0)
        return (seq_t)n;

    /* Check whether n is a pure power of that prime p. */
    idx_t tmp = n;
    while (tmp % p == 0)
        tmp /= p;

    return (tmp == 1) ? (seq_t)p : (seq_t)1;
}
