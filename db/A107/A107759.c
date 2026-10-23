/*
 * A107759  a(n) = (+2)UnitarySigma(n): if n = Product p_i^r_i then a(n) =
 *          Product (2 + p_i^r_i).
 * https://oeis.org/A107759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Product_{p^r || n} (2 + p^r) (OEIS A107759)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Treat n as a 64‑bit unsigned integer for factorisation. */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Trial division: factorise m into prime powers. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* Compute p^exp in seq_t. */
            seq_t power = (seq_t)1;
            for (uint64_t i = 0; i < exp; ++i) {
                power *= (seq_t)p;
            }
            /* Multiply the term (2 + p^exp). */
            result *= (seq_t)2 + power;
        }
    }

    /* If m > 1, it is a prime factor > sqrt(original n). */
    if (m > 1) {
        seq_t power = (seq_t)m;          /* exponent is 1 */
        result *= (seq_t)2 + power;
    }

    return result;
}
