/*
 * A077101  a(n) = A051612(n)*A065387(n) = sigma(n)^2-phi(n)^2, where
 *          A051612(n) = sigma(n) - phi(n) and A065387(n) = sigma(n) + phi(n).
 * https://oeis.org/A077101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n)^2 - phi(n)^2
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* sigma(n) and phi(n) by prime factorisation.
       Works for any n >= 1; trivial for n == 1.          */
    if (n == 0)
        return 0;

    seq_t sigma = 1;        /* σ(n)   */
    seq_t phi   = (seq_t)n; /* φ(n)   */
    idx_t    m   = n;       /* working copy */

    /* trial division up to √m */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            /* factor for σ(n) : 1 + p + p^2 + … + p^exp   */
            seq_t term = 1;          /* p^0  */
            seq_t ppow = 1;          /* running value of p^k  */
            for (idx_t k = 0; k < exp; ++k) {
                ppow *= (seq_t)p;    /* p^(k+1)  */
                term += ppow;
            }
            sigma *= term;

            /* update φ(n) : φ = φ – φ/p  (for each distinct prime) */
            phi -= phi / (seq_t)p;
        }
    }

    /* remainder is a prime factor > √n (exponent 1) */
    if (m > 1) {
        seq_t p = (seq_t)m;
        sigma *= (1 + p);
        phi -= phi / p;
    }

    /* a(n) = σ(n)^2 – φ(n)^2  */
    return sigma * sigma - phi * phi;
}
