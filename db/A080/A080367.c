/*
 * A080367  Largest unitary prime divisor of n or a(n) = 0 if no such prime
 *          divisor exists.
 * https://oeis.org/A080367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest unitary prime divisor of n or 0 if no such
 *            divisor exists. A unitary prime divisor is a prime p dividing n
 *            with exponent exactly 1 (i.e. p^1 | n and p^1 is a unitary
 *            divisor, meaning gcd(p, n/p)=1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Cast index to unsigned 64‑bit for factorisation. */
    uint64_t m = (uint64_t)n;
    seq_t maxp = 0;   /* largest unitary prime divisor found so far */

    if (m == 0) return 0;   /* safety, though n ≥ 1 in the OEIS offset */

    /* Handle factor 2 separately. */
    uint64_t exp2 = 0;
    while ((m & 1) == 0) {  /* while m % 2 == 0 */
        m >>= 1;            /* m /= 2 */
        ++exp2;
    }
    if (exp2 == 1 && (seq_t)2 > maxp) {
        maxp = 2;
    }

    /* Trial division by odd numbers up to sqrt(m). */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp == 1 && (seq_t)p > maxp) {
                maxp = p;
            }
        }
    }

    /* If remaining m > 1, it is a prime factor with exponent 1. */
    if (m > 1 && (seq_t)m > maxp) {
        maxp = m;
    }

    return maxp;
}
