/*
 * A140727  Expansion of (phi(q) * phi(q^15) - phi(q^3) * phi(q^5)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A140727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with * a(1) = 1 * a(2^e) = (-1)^e * (e-1)
 *            for e>0 * a(3^e) = a(5^e) = (-1)^e * a(p^e) = e+1 if p ≡ 1 or 4
 *            mod 15 * a(p^e) = (-1)^e * (e+1) if p ≡ 2 or 8 mod 15 * a(p^e) =
 *            (1+(-1)^e)/2 if p ≡ 7,11,13,14 mod 15 * The function computes
 *            a(n) for any n ≥ 1
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t prime_power_value(uint64_t p, uint64_t e) {
    /* Compute a(p^e) according to the rules above.  e > 0. */
    if (p == 2) {
        /* a(2^e) = (-1)^e * (e-1) */
        seq_t sign = (e % 2 == 0) ? 1 : -1;
        return sign * (seq_t)(e - 1);
    } else if (p == 3 || p == 5) {
        /* a(3^e) = a(5^e) = (-1)^e */
        seq_t sign = (e % 2 == 0) ? 1 : -1;
        return sign;
    } else {
        uint64_t mod = p % 15;
        if (mod == 1 || mod == 4) {
            /* a(p^e) = e+1                  if p ≡ 1 or 4 (mod 15) */
            return (seq_t)(e + 1);
        } else if (mod == 2 || mod == 8) {
            /* a(p^e) = (-1)^e * (e+1)       if p ≡ 2 or 8 (mod 15) */
            seq_t sign = (e % 2 == 0) ? 1 : -1;
            return sign * (seq_t)(e + 1);
        } else { /* mod == 7,11,13,14 */
            /* a(p^e) = (1+(-1)^e)/2        if p ≡ 7,11,13,14 (mod 15) */
            return (e % 2 == 0) ? 1 : 0;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n<1, return 0 */
    uint64_t x = (uint64_t)n;
    seq_t result = 1;
    /* Handle factor 2 separately for speed */
    if ((x & 1ULL) == 0) {
        uint64_t e = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            ++e;
        }
        result *= prime_power_value(2ULL, e);
    }
    /* Iterate over odd potential divisors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            result *= prime_power_value(p, e);
        }
    }
    /* Remaining prime factor, if any */
    if (x > 1) {
        result *= prime_power_value(x, 1);
    }
    return result;
}
