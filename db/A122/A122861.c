/*
 * A122861  Expansion of phi(-q)chi(-q)psi(q^3) in powers of q where
 *          phi(),chi(),psi() are Ramanujan theta functions.
 * https://oeis.org/A122861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(3n+1) where b is multiplicative, defined on prime
 *            powers as: b(2^e) = -3 if e even, 0 if e odd b(3^e) = 0 if e > 0
 *            b(p^e) = e+1 if p ≡ 1 (mod 6) b(p^e) = (1+(-1)^e)/2 if p ≡ 5
 *            (mod 6) b(1) = 1 The function factors 3n+1 and multiplies the
 *            appropriate contributions. No external libraries a...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = 3*n + 1;          /* number to factor */
    seq_t res = 1;

    /* factor out powers of 2 */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        if (e & 1)                /* odd exponent -> factor 0 */
            return 0;
        res *= (seq_t)-3;         /* even exponent -> factor -3 */
    }

    /* factor out powers of 3: any positive power makes the result 0 */
    if (m % 3 == 0)
        return 0;                 /* 3^e for e>0 gives zero product */

    /* factor the remaining odd primes > 3 via trial division */
    for (idx_t d = 5; d <= m / d; d += 2) {
        if (m % d == 0) {
            idx_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            /* Compute the contribution of prime d according to its residue mod 6 */
            if (d % 6 == 1) {          /* p ≡ 1 (mod 6) -> factor = e+1 */
                res *= (seq_t)(e + 1);
            } else { /* d % 6 == 5 */
                if (e & 1)              /* odd exponent -> factor 0 */
                    return 0;
                /* even exponent gives factor 1 – nothing to do */
            }
            if (res == (seq_t)0)        /* early exit */
                return 0;
        }
    }

    /* if something remains, it is a prime > sqrt(original m) */
    if (m > 1) {
        /* m is prime.  Compute its contribution. */
        if (m % 6 == 1) {              /* p ≡ 1 (mod 6) -> e = 1 -> factor 2 */
            res *= (seq_t)2;
        } else if (m % 6 == 5) {       /* p ≡ 5 (mod 6) -> odd exponent -> factor 0 */
            return 0;
        }
        /* m == 2 or 3 cannot occur here (handled earlier) */
    }

    return res;
}
