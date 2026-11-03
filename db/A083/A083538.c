/*
 * A083538  a(n) = sigma(n)*sigma(n+1)/gcd(sigma(n+1), sigma(n))^2.
 * https://oeis.org/A083538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)*sigma(n+1)/gcd(sigma(n+1), sigma(n))^2
 * Range:     u8 n <= 13, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9955 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of divisors of n.  n ≥ 1. */
static seq_t sigma_of_idx(idx_t n)
{
    if (n == 0) return 0;          /* unused, but keeps the compiler happy */

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* compute the factor (p^(e+1)-1)/(p-1) without overflow */
            seq_t term = 1;
            seq_t pow  = 1;
            while (m % p == 0) {
                m   /= p;
                pow  *= (seq_t)p;
                term += pow;
            }
            result *= term;
        }
    }

    /* if a prime factor larger than sqrt(n) remains */
    if (m > 1) {
        result *= (seq_t)1 + (seq_t)m;
    }

    return result;
}

/* Euclidean algorithm on seq_t.  Works for any sized unsigned type. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, not used */

    seq_t sigma_n  = sigma_of_idx(n);
    seq_t sigma_n1 = sigma_of_idx(n + (idx_t)1);

    seq_t g  = gcd_seq_t(sigma_n, sigma_n1);
    seq_t g2 = g * g;

    return (sigma_n * sigma_n1) / g2;
}
