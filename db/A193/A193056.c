/*
 * A193056  Reciprocals are the complement to logarithm of Riemann zeta.
 *          a(1)=0, for n>1: a(n) = A008683(n) + A100995(n).
 * https://oeis.org/A193056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if n = 1 or n is prime, e if n = p^e (p prime, e ≥ 2),
 *            1 if n = p * q (p, q distinct primes), -1 if n is square‑free
 *            with at least 3 distinct prime factors, 0 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1.  The sequence is defined by the OEIS formula
   a(n) = A008683(n) + A100995(n), which reduces to the simple case
   analysis below.  The implementation uses only integer arithmetic and
   allows seq_t to be signed.  */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    idx_t temp = n;
    idx_t distinct = 0;   /* number of distinct prime factors ω(n) */
    int total_exp = 0;    /* total exponent Ω(n) */
    int has_exponent_gt1 = 0; /* flag if some exponent > 1 */

    /* Trial division up to sqrt(temp).  Using p <= temp/p avoids
       overflow in the multiplication p*p. */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            ++distinct;
            int e = 0;
            do {
                temp /= p;
                ++e;
            } while (temp % p == 0);
            total_exp += e;
            if (e > 1)
                has_exponent_gt1 = 1;
        }
    }

    /* If anything remains, it is a prime factor with exponent 1. */
    if (temp > 1) {
        ++distinct;
        total_exp += 1;
    }

    /* Prime power n = p^e */
    if (distinct == 1) {
        if (total_exp == 1)          /* prime itself */
            return (seq_t)0;
        else                          /* prime power, e ≥ 2 */
            return (seq_t)total_exp;
    }

    /* Two distinct prime factors */
    if (distinct == 2) {
        if (total_exp == 2)          /* both exponents 1 -> n = p*q */
            return (seq_t)1;
        else                          /* one exponent > 1 -> 0 */
            return (seq_t)0;
    }

    /* Three or more distinct prime factors.  If n is square‑free,
       then total_exp == distinct and a(n) = -1; otherwise a(n) = 0. */
    if (distinct >= 3) {
        if (total_exp == distinct)   /* square‑free with ω(n) ≥ 3 */
            return (seq_t)(-1);
        else
            return (seq_t)0;
    }

    /* Should never reach here, but for completeness return 0. */
    return (seq_t)0;
}
