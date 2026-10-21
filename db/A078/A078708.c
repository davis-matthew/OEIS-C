/*
 * A078708  Sum of divisors d of n such that n/d is not congruent to 0 mod 3.
 * https://oeis.org/A078708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = (sigma(3*n) - sigma(n)) / 3 Using the multiplicative
 *            property of the sum of divisors, the value can be computed by
 *            factoring out powers of 3 from n. Let n = 3^e * m where 3 ∤ m.
 *            Then sigma(3n) - sigma(n) = 3^(e+1) * sigma(m), and a(n) = 3^e *
 *            sigma(m). The function below implements that me...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Remove all factors of 3 from n to obtain m and the exponent e. */
    idx_t m = n;
    idx_t e = 0;
    while (m % 3 == 0) {
        m /= 3;
        e++;
    }

    /* Compute sigma(m) = sum of divisors of m. */
    seq_t sigma_m = 0;
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            sigma_m += (seq_t)i;
            idx_t j = m / i;
            if (j != i) {
                sigma_m += (seq_t)j;
            }
        }
    }

    /* Compute 3^e. */
    seq_t pow3e = 1;
    for (idx_t k = 0; k < e; ++k) {
        pow3e *= (seq_t)3;
    }

    /* Final result: a(n) = sigma(m) * 3^e. */
    return sigma_m * pow3e;
}
