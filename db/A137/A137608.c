/*
 * A137608  Expansion of (1 - psi(-q)^3 / psi(-q^3)) / 3 in powers of q where
 *          psi() is a Ramanujan theta function.
 * https://oeis.org/A137608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1. a(n) is multiplicative with a(2^e) = -1 (e > 0) a(3^e)
 *            = 1 a(p^e) = e + 1 if p ≡ 1 (mod 6) a(p^e) = (1 + (-1)^e)/2 if p
 *            ≡ 5 (mod 6) (for primes p > 3). For 2 and 3 the above rules
 *            apply as stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t x = n;              /* work on unsigned 64‑bit n */
    seq_t res = 1;               /* multiplicative identity */

    /* handle prime 2 */
    if (x % 2 == 0) {
        res = -1;                /* a(2^e) = -1 for e>0 */
        while (x % 2 == 0) x /= 2;
    }

    /* handle prime 3 */
    while (x % 3 == 0) {
        /* a(3^e) = 1 – nothing to multiply */
        x /= 3;
    }

    if (x == 1) return res;      /* finished with small factors */

    /* factor the remaining part using trial division */
    for (uint64_t d = 5; d <= x / d; d += 2) {  /* d odd */
        if (x % d == 0) {
            int exp = 0;
            while (x % d == 0) {
                x /= d;
                exp++;
            }

            if (d % 6 == 1) {            /* p ≡ 1 (mod 6) */
                res *= (seq_t)(exp + 1);
            } else if (d % 6 == 5) {     /* p ≡ 5 (mod 6) */
                /* (1 + (-1)^exp)/2; it is 0 for odd exp */
                if (exp & 1) return (seq_t)0;
                /* even exp contributes factor 1 – no change */
            }
        }
    }

    /* if residual part > 1 it is prime and > 3 */
    if (x > 1) {
        if (x % 6 == 1) {
            res *= (seq_t)2;          /* exponent 1 → e+1 = 2 */
        } else if (x % 6 == 5) {
            /* exponent 1 (odd), so value becomes zero */
            return (seq_t)0;
        }
        /* other cases are impossible because all 2 and 3 factors are removed */
    }

    return res;
}
