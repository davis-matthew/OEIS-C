/*
 * A062955  a(n) = phi(n^2) - phi(n) = (n-1) * phi(n).
 * https://oeis.org/A062955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * phi(n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Euler's totient φ(n) by prime factorisation. */
    seq_t phi_n = (seq_t)n;       /* φ(m) initially equals m itself */
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            phi_n -= phi_n / (seq_t)p;  /* φ(n) = φ(n) * (1 - 1/p) */
            while (m % p == 0)          /* strip all factors of p */
                m /= p;
        }
    }
    /* If a prime factor greater than sqrt(n) remains, account for it. */
    if (m > 1)
        phi_n -= phi_n / (seq_t)m;

    /* a(n) = (n-1) * φ(n).  Cast (n-1) to seq_t to avoid overflow on idx_t. */
    return ((seq_t)(n - 1)) * phi_n;
}
