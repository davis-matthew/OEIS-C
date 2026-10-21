/*
 * A209940  Expansion of psi(x^4) * phi(-x^4)^4 / phi(x) in powers of x where
 *          phi(), psi() are Ramanujan theta function.
 * https://oeis.org/A209940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209940 – Expansion of ψ(x⁴)·φ(−x⁴)⁴ / φ(x).
   The sequence is given by
       a(n) = b(2n+1),
   where b is multiplicative and
       b(3^e) = (1−(−3)^{e+1})/4,
       for an odd prime p≠3 and exponent e≥1
       b(p^e) = (−1)^{e·((p−1)/4)} · [(p·f)^{e+1}−1]/(p·f−1),
   with f = Kronecker(18,p) = (2/p) (Legendre symbol of 2 mod p).
   The code below factors 2n+1 and applies the above formulas.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* m = 2n+1 is always odd */
    uint64_t m = (uint64_t)2 * n + 1;
    seq_t result = 1;
    uint64_t temp = m;

    /* Handle factor 3 separately */
    if (temp % 3 == 0) {
        uint8_t e = 0;
        do { temp /= 3; ++e; } while (temp % 3 == 0);
        seq_t pow_neg3 = seq_pow((seq_t)-3, (idx_t)(e + 1));
        seq_t contrib = (1 - pow_neg3) / 4;
        result *= contrib;
    }

    /* Process the remaining odd factors */
    for (uint64_t p = 5; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        uint8_t e = 0;
        do { temp /= p; ++e; } while (temp % p == 0);

        /* f = (2/p) = 1 if p≡1,7 (mod 8), else −1 */
        seq_t f = ((p % 8 == 1 || p % 8 == 7) ? 1 : -1);

        /* t = p·f  (may be negative) */
        seq_t t = (seq_t)p * f;

        /* Compute the geometric sum 1 + t + t² + … + t^e */
        seq_t sum = 1, term = 1;
        for (uint8_t k = 1; k <= e; ++k) {
            term = term * t;
            sum += term;
        }

        /* Sign factor (−1)^{e·((p−1)/4)} */
        uint8_t sign_exp = e * (uint8_t)((p - 1) / 4);
        if (sign_exp & 1) sum = -sum;

        result *= sum;
    }

    /* Any remaining prime factor > sqrt(m) */
    if (temp > 1) {
        uint64_t p = temp;
        uint8_t e = 1;                   /* exponent is 1 */

        seq_t f = ((p % 8 == 1 || p % 8 == 7) ? 1 : -1);
        seq_t t = (seq_t)p * f;
        seq_t sum = 1 + t;               /* e == 1 */

        uint8_t sign_exp = (uint8_t)((p - 1) / 4);
        if (sign_exp & 1) sum = -sum;

        result *= sum;
    }

    return result;
}
