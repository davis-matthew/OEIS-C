/*
 * A122864  Expansion of eta(q^3)^2 * eta(q^4) * eta(q^6)^2 * eta(q^36) /
 *          (eta(q) * eta(q^9) * eta(q^12)^2) in powers of q.
 * https://oeis.org/A122864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative function defined by the following
 *            prime‑power rules: - a(2^e) = 1 (e ≥ 0) - a(3^e) = 2 *
 *            (-1)^(e+1) (e > 0) - for primes p ≡ 1 (mod 4): a(p^e) = e + 1 -
 *            for primes p ≡ 3 (mod 4): a(p^e) = (1 + (-1)^e) / 2 The value of
 *            a(n) for composite n is the product of its values on each...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the multiplicative sequence value for n >= 1.
   Uses trial division.  For very large n the cost may grow,
   but for n up to 10^8 it is still trivial.  */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;           /* use unsigned 64-bit for factorisation */
    uint64_t p = 2;

    while (m > 1) {
        /* Find smallest prime factor p of m */
        while (p * p <= m && m % p != 0) {
            ++p;
        }
        if (p * p > m) {                /* m itself is prime */
            p = m;
        }

        /* Count exponent e of prime p in m */
        unsigned e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* Compute contribution of p^e according to rules */
        seq_t factor = 1;
        if (p == 2) {
            factor = 1;                 /* a(2^e) = 1 */
        } else if (p == 3) {
            /* a(3^e) = 2 * (-1)^(e+1) */
            seq_t sign = ((e + 1) & 1) ? -1 : 1;  /* (-1)^(e+1) */
            factor = 2 * sign;
        } else if ((p % 4) == 1) {
            factor = (seq_t)(e + 1);    /* a(p^e) = e+1 */
        } else {                         /* p % 4 == 3 */
            factor = (seq_t)(((e & 1) == 0) ? 1 : 0);
        }

        result *= factor;
        if (result == 0) break;          /* no need to multiply further */
        p = 2;                           /* restart search for next factor */
    }

    return result;
}
