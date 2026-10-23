/*
 * A124449  Expansion of (phi(-q^3)^4 - phi(-q)^4)/8 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A124449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p^e) where f(2^e) = -3 (e>0) f(3^e) = 3^e
 *            f(p^e) = (p^(e+1)-1)/(p-1) for p>3
 * Range:     i8 n <= 48, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute the A124449 sequence value for index n (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined below the offset – return 0 or any value */
        return 0;
    }

    seq_t res = 1;
    uint64_t m = n;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        res = -3;                     /* f(2^e) = -3 for any e>0 */
        while ((m & 1ULL) == 0) m >>= 1;
    }

    /* process odd primes, starting from 3 */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }

            if (p == 3) {
                /* f(3^e) = 3^e */
                seq_t pow3 = 1;
                for (int i = 0; i < e; ++i) pow3 *= (seq_t)3;
                res *= pow3;
            } else {
                /* f(p^e) = (p^(e+1)-1)/(p-1)  for p>3 */
                seq_t pow = 1;
                for (int i = 0; i < e + 1; ++i) pow *= (seq_t)p;
                seq_t term = (pow - 1) / ((seq_t)p - 1);
                res *= term;
            }
        }
    }

    /* remaining prime factor > sqrt(m), if any */
    if (m > 1) {
        if (m == 3) {
            res *= 3;                       /* last factor 3^1 */
        } else {
            /* for p>3, e=1: f(p) = (p^2-1)/(p-1) = p+1 */
            res *= ((seq_t)m + 1);
        }
    }

    return res;
}
