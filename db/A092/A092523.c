/*
 * A092523  Number of distinct prime factors of n-th odd number.
 * https://oeis.org/A092523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001221(2*n-1) – number of distinct prime factors of the
 *            n-th odd number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based. For n == 0 the value is undefined; return 0 to avoid issues. */
    if (n == 0)
        return 0;

    /* Compute the n-th odd number: 2*n-1 */
    idx_t m = (n << 1) - 1;     /* use shift for 2*n */

    seq_t count = 0;

    /* Special case: m == 1 has zero prime factors */
    if (m == 1)
        return 0;

    /* Factorization using trial division.
       Since m is odd, we skip 2.  We test only odd divisors up to sqrt(m). */
    /* Count factor 2 if present (only for completeness, though m is odd). */
    if ((m & 1) == 0) {
        count++;
        while ((m & 1) == 0)
            m >>= 1;
    }

    /* Test odd divisors i = 3,5,7,... */
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            count++;
            while (m % i == 0)
                m /= i;
        }
    }

    /* If remaining m > 1, it is a prime factor. */
    if (m > 1)
        count++;

    return count;
}
