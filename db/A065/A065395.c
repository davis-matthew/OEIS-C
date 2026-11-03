/*
 * A065395  Commutator of sigma and phi functions.
 * https://oeis.org/A065395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(phi(n)) - phi(sigma(n))
 * Range:     i8 n <= 59, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* ------------------------------------------------------------------ */
static seq_t my_sigma(idx_t n)
{
    if (n == 0) return 0;          /* not used for offset ≥ 1 */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {  /* avoids i*i overflow */
        if (n % i == 0) {
            seq_t j = n / i;
            sum += (seq_t)i;
            if (j != i) {
                sum += (seq_t)j;
            }
        }
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Helper: Euler's totient function: φ(m)                              */
/* ------------------------------------------------------------------ */
static seq_t my_phi(idx_t m)
{
    if (m == 0) return 0;          /* not used for offset ≥ 1 */
    seq_t result = (seq_t)m;
    idx_t tmp = m;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            /* p is a prime factor of m */
            while (tmp % p == 0) {
                tmp /= p;
            }
            result -= result / (seq_t)p;
        }
    }
    if (tmp > 1) {                  /* remaining prime factor > sqrt(m) */
        result -= result / (seq_t)tmp;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Helper: Sum-of-divisors function: σ(n)                              */

/* ------------------------------------------------------------------ */
/* Sequence function: a(n)                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t phi_n     = my_phi(n);
    seq_t sigma_phi = my_sigma((idx_t)phi_n);

    seq_t sigma_n   = my_sigma(n);
    seq_t phi_sigma = my_phi((idx_t)sigma_n);

    return sigma_phi - phi_sigma;
}
