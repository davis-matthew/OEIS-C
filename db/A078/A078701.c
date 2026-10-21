/*
 * A078701  Least odd prime factor of n, or 1 if no such factor exists.
 * https://oeis.org/A078701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd prime factor of n, or 1 if none
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; for n > 1, a(n) = 1 if n is a power of 2, otherwise
   the smallest odd prime dividing n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    /* Remove all factors of 2.  If the result is 1, n is a power of two. */
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }
    if (m == 1) {
        return 1;
    }

    /* Find the smallest odd divisor of m.  Since m is odd, start from 3
       and check only odd candidates.  The loop runs at most sqrt(m) times,
       and for 64‑bit values sqrt(m) ≤ 4·10⁹, which is safe for 64‑bit
       arithmetic. */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            return (seq_t)p;
        }
    }

    /* If no divisor was found, m is prime.  Return it. */
    return (seq_t)m;
}
