/*
 * A121454  Expansion of q * psi(-q) * psi(-q^7) in powers of q where psi(q)
 *          is a Ramanujan theta function.
 * https://oeis.org/A121454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(2^e) = -1 for e>0, a(7^e) = 1,
 *            a(p^e) = e+1 if p ≡ 1,2,4 (mod 7), a(p^e) = (1+(-1)^e)/2 if p ≡
 *            3,5,6 (mod 7)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n ≥ 1 */
    idx_t m = n;
    seq_t prod = 1;

    /* handle factor 2 */
    if ((m & 1) == 0) {
        prod = -prod;                 /* multiply by -1 once */
        while ((m & 1) == 0) m >>= 1; /* remove all powers of 2 */
    }

    /* factor primes ≥ 3 */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (p == 7) {
                /* multiplier 1, nothing to do */
                continue;
            }
            int mod = (int)(p % 7);
            if (mod == 1 || mod == 2 || mod == 4) {
                prod *= (seq_t)(e + 1);
            } else if (mod == 3 || mod == 5 || mod == 6) {
                if (e & 1)          /* odd exponent → zero factor */
                    return 0;
                /* even exponent → multiplier 1, nothing to do */
            }
        }
    }

    /* handle remaining prime factor > sqrt(original n) */
    if (m > 1) {
        /* m is prime */
        if (m == 7) {
            /* multiplier 1, nothing to do */
        } else {
            int mod = (int)(m % 7);
            if (mod == 1 || mod == 2 || mod == 4) {
                prod *= (seq_t)2;   /* exponent e = 1 → e+1 = 2 */
            } else if (mod == 3 || mod == 5 || mod == 6) {
                /* exponent 1 odd → zero factor */
                return 0;
            }
        }
    }
    return prod;
}
