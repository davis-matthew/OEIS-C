/*
 * A077099  a(n) = gcd(A051612(n), A065387(n)), where A051612(n) = sigma(n) -
 *          phi(n) and A065387(n) = sigma(n) + phi(n).
 * https://oeis.org/A077099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( sigma(n)-phi(n), sigma(n)+phi(n) )
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* not defined for n=0, return 0 just in case */

    idx_t m = n;
    seq_t sigma = 1;
    seq_t phi   = (seq_t)n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t p_pow   = 1;
            seq_t sigma_p = 1;  /* 1 + p + p^2 + ... */
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                sigma_p += p_pow;
            }
            sigma *= sigma_p;
            phi   -= phi / (seq_t)p;
        }
    }

    if (m > 1) { /* remaining prime factor */
        sigma *= (1 + (seq_t)m);
        phi   -= phi / (seq_t)m;
    }

    seq_t a_val = sigma - phi;
    seq_t b_val = sigma + phi;

    /* Euclidean algorithm for gcd */
    seq_t x = a_val, y = b_val;
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}
