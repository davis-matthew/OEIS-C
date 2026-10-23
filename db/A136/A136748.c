/*
 * A136748  Expansion of (a(q) - a(q^2) - 4*a(q^4) + 4*a(q^8)) / 6 in powers
 *          of q where a() is a cubic AGM theta function.
 * https://oeis.org/A136748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   the sequence is multiplicative. * a(1) = 1 * a(2) = -1, a(2^e) =
 *            -3 * (-1)^e for e > 1 * a(3^e) = 1 * a(p^e) = e + 1 if p ≡ 1
 *            (mod 6) (p odd prime) * a(p^e) = (1 + (-1)^e)/2 if p ≡ 5 (mod 6)
 *            (p odd prime)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based;  a(1) = 1 will be produced by the following logic. */
    if (n == 0) return 0;           /* safety: should never happen */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* factor 2 */
    int e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        seq_t val;
        if (e == 1) {
            val = -1;               /* a(2) */
        } else {
            /* a(2^e) = -3 * (-1)^e  for e > 1 */
            seq_t sign = (e & 1) ? -1 : 1;   /* (-1)^e */
            val = -3 * sign;
        }
        result *= val;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t val;
            if (p == 3) {
                val = 1;                       /* a(3^e) = 1 */
            } else if (p % 6 == 1) {
                val = (seq_t)(e + 1);           /* a(p^e) = e+1 */
            } else if (p % 6 == 5) {
                val = (e & 1) ? 0 : 1;          /* a(p^e) = 0 if e odd, 1 if e even */
            } else {
                val = 1; /* should never occur */
            }
            result *= val;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        uint64_t p = m;
        seq_t val;
        if (p == 3) {
            val = 1;
        } else if (p % 6 == 1) {
            val = (seq_t)(1 + 1);   /* exponent e = 1, so e+1 = 2 */
        } else if (p % 6 == 5) {
            val = 0;                /* e = 1 is odd, so a(p) = 0 */
        } else {
            val = 1; /* safety */
        }
        result *= val;
    }

    return result;
}
