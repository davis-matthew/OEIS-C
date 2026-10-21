/*
 * A237649  a(n) = A163659(n^3), where A163659 is the logarithmic derivative
 *          of Stern's diatomic series (A002487).
 * https://oeis.org/A237649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = A163659(n^3). The function is multiplicative with *
 *            a(2^e) = 2^(3*e+1) - 1, * a(3^e) = -2, * a(p^e) = 1 for all
 *            primes p ≥ 5. * So only the exponents of 2 and 3 influence the
 *            result
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Separate the powers of 2 and 3 (the rest contribute a factor 1) */
    idx_t tmp = n;
    idx_t e2   = 0;
    idx_t e3   = 0;

    while (tmp % 2 == 0) {
        ++e2;
        tmp /= 2;
    }
    while (tmp % 3 == 0) {
        ++e3;
        tmp /= 3;
    }

    seq_t result = 1;

    /* Contribution from the power of two */
    if (e2 > 0) {
        /* Compute 2^(3*e2+1) */
        seq_t pow2 = 1;
        idx_t exp = 3 * e2 + 1;
        for (idx_t i = 0; i < exp; ++i) {
            pow2 *= (seq_t)2;
        }
        result *= (pow2 - (seq_t)1);
    }

    /* Contribution from the power of three */
    if (e3 > 0) {
        result *= (seq_t)-2;
    }

    return result;
}
