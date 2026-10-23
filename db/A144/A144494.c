/*
 * A144494  a(n) = 0 if n is prime, otherwise A001222(n).
 * https://oeis.org/A144494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is prime, otherwise the number of prime factors of
 *            n counted with multiplicity (big Omega). This is the function
 *            A001222(n)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* a(1) = 0 */
    uint64_t x = (uint64_t)n;     /* n fits in uint64_t */
    seq_t omega = 0;              /* count of prime factors, with multiplicity */

    /* Factorize n using the smallest prime factor routine */
    while (x > 1) {
        uint64_t spf = _smallest_prime_factor(x);
        if (spf == 0) break;      /* safety net – should not happen */
        omega++;
        x /= spf;
    }

    /* For prime numbers omega == 1 – replace by 0 per definition */
    return (omega <= 1) ? 0 : omega;
}
