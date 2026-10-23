/*
 * A124827  Order of Galois groups of irreducible Chebyshev polynomials of
 *          order n.
 * https://oeis.org/A124827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, more
 * Formula:   a(n) = n * φ(n) / g, where * φ(n) is Euler's totient function, *
 *            g = 2 if n is a power of two greater than 4, otherwise g = 1. *
 *            This matches OEIS entry A124827 for all n ≥ 1
 * Range:     u8 n <= 16, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Compute φ(n).  φ(1) = 1. */
    seq_t phi = (seq_t)n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* p is a prime factor of n. Strip all powers of p. */
            while (m % p == 0) m /= p;
            /* Update φ: φ ← φ / p * (p - 1) */
            phi = phi / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (m > 1) {           /* remaining prime factor larger than sqrt(n) */
        phi = phi / (seq_t)m * (seq_t)(m - 1);
    }

    /* Compute order a(n) = n * φ(n) / g. */
    seq_t result = (seq_t)n * phi;
    /* Determine if n is a power of two > 4. */
    bool power_of_two = (n & (n - 1)) == 0;      /* 1,2,4,8,... */
    if (power_of_two && n > 4) {
        result /= 2;   /* divide by g = 2 for powers of 2 > 4 */
    }

    return result;
}
