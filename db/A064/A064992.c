/*
 * A064992  a(n) = usigma(n+1) - usigma(n), where usigma(n) is the sum of
 *          unitary divisors of n (A034448).
 * https://oeis.org/A064992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = usigma(n+1) – usigma(n) where usigma(n) is the sum of
 *            unitary divisors of n (A034448)
 * Range:     i8 n <= 136, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8924 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t unitary_sigma(idx_t n)
{
    /* Sum of unitary divisors of n.
       For each prime power p^e dividing n,
       multiply the result by (1 + p^e). */
    if (n == 0) return 0;                 /* unused for n >= 1 */
    seq_t res = 1;                        /* product over prime factors */
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t p_pow = 1;
            do {
                m /= p;
                p_pow *= (seq_t)p;
            } while (m % p == 0);
            res *= ((seq_t)1 + p_pow);
        }
    }
    if (m > 1) {                          /* remaining prime factor > sqrt(n) */
        res *= ((seq_t)1 + (seq_t)m);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute a(n) = usigma(n+1) - usigma(n) */
    seq_t sigma_n     = unitary_sigma(n);
    seq_t sigma_nplus = unitary_sigma(n + 1);
    return sigma_nplus - sigma_n;
}
