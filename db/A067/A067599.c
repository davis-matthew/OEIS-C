/*
 * A067599  Decimal encoding of the prime factorization of n: concatenation of
 *          prime factors and exponents.
 * https://oeis.org/A067599
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal encoding of the prime factorization of n. For
 *            each prime factor p_i and its exponent e_i (p_1 < … < p_r), the
 *            encoding is p_1 followed by e_1, then p_2 followed by e_2, ….
 *            Example: n = 15 = 3¹·5¹ → a(15) = 3151
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_u64.h"

/* Count the number of decimal digits of an unsigned integer. */
static int digits_idx(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2. */
    if (n < 2) return 0;

    seq_t result = 0;
    idx_t m = n;
    idx_t p, exp;

    /* Handle prime 2 first (optional but speeds up). */
    if ((m & 1) == 0) {          /* m % 2 == 0 */
        exp = 0;
        while ((m & 1) == 0) {   /* while m % 2 == 0 */
            ++exp;
            m >>= 1;             /* m /= 2 */
        }
        int digits_p = 1;                 /* '2' has one digit */
        int digits_e = digits_idx(exp);
        seq_t part = (seq_t)2 * pow10_u64(digits_e) + exp;
        int shift = digits_p + digits_e;
        result = result * pow10_u64(shift) + part;
    }

    /* Test odd divisors up to sqrt(m). */
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            int digits_p = digits_idx(p);
            int digits_e = digits_idx(exp);
            seq_t part = (seq_t)p * pow10_u64(digits_e) + exp;
            int shift = digits_p + digits_e;
            result = result * pow10_u64(shift) + part;
        }
    }

    /* If a remainder > 1, it is a prime factor with exponent 1. */
    if (m > 1) {
        int digits_p = digits_idx(m);
        int digits_e = 1;                     /* exponent 1 */
        seq_t part = (seq_t)m * pow10_u64(digits_e) + 1;
        int shift = digits_p + digits_e;
        result = result * pow10_u64(shift) + part;
    }

    return result;
}
