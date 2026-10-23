/*
 * A125029  a(n) = number of exponents in the prime factorization of n that
 *          are noncomposite.
 * https://oeis.org/A125029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of exponents in the prime factorization of n that
 *            are 1 or prime (non‑composite)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Small helper: test primality of a uint64_t exponent (small values are enough) */
static inline bool is_prime_exponent(uint64_t e) {
    if (e < 2) return false;
    for (uint64_t d = 2; d * d <= e; ++d) {
        if (e % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* offset is 1 */
    if (n == 1) return (seq_t)0;

    uint64_t m = (uint64_t)n;   /* work in a native integer type */
    seq_t count = 0;

    /* factor 2 */
    uint64_t exp = 0;
    while ((m & 1ULL) == 0ULL) {
        ++exp;
        m >>= 1;
    }
    if (exp > 0 && (exp == 1 || is_prime_exponent(exp))) {
        ++count;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            if (exp == 1 || is_prime_exponent(exp)) {
                ++count;
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* exponent is 1 */
        ++count;
    }

    return count;
}
