/*
 * A117569  Expansion of (1+x+x^2)/(1+x^2).
 * https://oeis.org/A117569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) is the real Dirichlet character modulo 4. a(0) = 1; if n is
 *            even (n>0) then a(n) = 0; else a(n) = (-1)^{sum of exponents of
 *            primes congruent to 3 (mod 4)}. Thus a is completely
 *            multiplicative with a(1)=1, a(p)=1 if p≡1 (mod 4), a(p)=−1 if
 *            p≡3 (mod 4)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if ((n & 1u) == 0) { /* even n > 0 */
        return (seq_t)0;
    }

    seq_t sign = (seq_t)1;
    idx_t m = n;

    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (p % 4 == 3) {
                if (exp & 1) {
                    sign = -sign;
                }
            }
        }
    }

    /* Remaining factor m > 1 is a prime (odd) with exponent 1. */
    if (m > 1) {
        if (m % 4 == 3) {
            sign = -sign;
        }
    }

    return sign;
}
