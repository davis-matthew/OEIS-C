/*
 * A161882  Smallest k such that n^2 = a_1^2 + ... + a_k^2 and all a_i are
 *          positive integers less than n.
 * https://oeis.org/A161882
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 700, u16 n <= 700, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = 4 if n is a power of two;
   a(n) = 2 if n has at least one prime factor ≡ 1 (mod 4);
   otherwise a(n) = 3.  This implements OEIS A161882. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;   /* value for indices below the offset */
    }

    /* Power‑of‑two case: n is a 2‑power  → a(n)=4 */
    if ((n & (n - 1)) == 0) {
        return (seq_t)4;
    }

    idx_t m = n;

    /* Remove all factors of 2 (they cannot contribute a prime ≡1 (mod 4)) */
    while ((m & 1U) == 0) {
        m >>= 1;
    }

    /* Trial division by odd numbers up to sqrt(m) */
    for (idx_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            if (d % 4U == 1U) {
                return (seq_t)2;
            }
            /* Strip out all copies of this divisor to keep m small */
            while (m % d == 0) {
                m /= d;
            }
        }
    }

    /* If a prime factor > sqrt(n) remains */
    if (m > 1 && m % 4U == 1U) {
        return (seq_t)2;
    }

    /* No prime factor ≡1 (mod 4) found → a(n)=3 */
    return (seq_t)3;
}
