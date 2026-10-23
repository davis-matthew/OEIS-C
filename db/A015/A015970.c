/*
 * A015970  Inverse of 1961st cyclotomic polynomial.
 * https://oeis.org/A015970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ_1961(x), where Φ_1961 is the 1961‑st cyclotomic polynomial.
 *            The sequence is periodic with period 1961
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool initialized = false;
static seq_t a_values[1961];
static seq_t b_coeff[1873];   /* coefficients of Φ_1961(x), degree 1872 */

/* Compute coefficients of Φ_1961(x) using the identity
   Φ_{pq}(x) = Φ_p(x^q) / Φ_p(x)  for distinct primes p,q  (here 37,53). */
static void init_sequence(void)
{
    if (initialized)
        return;

    /* parameters: 1961 = 37 * 53 */
    const int p = 37;
    const int q = 53;
    const int degB = p - 1;                     /* degree of Φ_p(x) */
    const int degA = q * degB;                  /* degree of Φ_p(x^q)   */
    const int degQ = (q - 1) * degB;            /* degree of Φ_{pq}(x) = φ(1961) */

    /* long division: Φ_{pq}(x) = Φ_p(x^q) / Φ_p(x) */
    for (int k = 0; k <= degQ; ++k) {
        seq_t Ak = ((k <= degA) && (k % q == 0)) ? (seq_t)1 : (seq_t)0;

        int start = k - degB;
        if (start < 0)
            start = 0;
        seq_t sum = 0;
        for (int i = start; i < k; ++i)
            sum += b_coeff[i];          /* B[j] == 1 for 0 <= j <= degB */

        b_coeff[k] = Ak - sum;
    }

    /* Recurrence for the inverse series:
       a(0) = 1
       a(k) = - Σ_{j=1}^{min(k,φ)} a(k-j) * b(j)   for k ≥ 1         */
    const int period = 1961;
    a_values[0] = (seq_t)1;
    const int degPhi = degQ;                    /* φ(1961) = 1872 */

    for (int k = 1; k < period; ++k) {
        seq_t sum = 0;
        int maxj = (k <= degPhi) ? k : degPhi;
        for (int j = 1; j <= maxj; ++j)
            sum += a_values[k - j] * b_coeff[j];
        a_values[k] = -sum;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_sequence();
    return a_values[(unsigned)(n % 1961)];
}
