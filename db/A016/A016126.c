/*
 * A016126  Inverse of 2117th cyclotomic polynomial.
 * https://oeis.org/A016126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of 1/Φ_2117(x), where Φ_2117(x) is the 2117th cyclotomic
 *            polynomial. The sequence is periodic with period 2117. The
 *            coefficients a(0),a(1),...,a(23) are all 1 as given in the OEIS
 *            entry. The implementation pre‑computes the coefficient...
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/*  Φ_2117(x) = Φ_29(x^73)/Φ_29(x)  with Φ_29(x) = 1+x+...+x^28
    The degree of Φ_2117 is 2016, and the series inverse of Φ_2117 is
    periodic of length 2117. */
#define PERIOD      2117
#define DEG 2016            /* degree of Φ_2117 */

/* Compute the integer coefficients of Φ_2117(x) into coeff[0..DEG] */
static void compute_phi_coeffs(int coeff[DEG + 1])
{
    const int p = 29;          /* smaller prime factor */
    const int q = 73;          /* larger prime factor */

    /* Φ_29(x) coefficients: a single array of 29 ones */
    int den[29];
    for (int i = 0; i < 29; ++i) den[i] = 1;

    /* Numerator Φ_29(x^q) coefficients:
       1 at multiples of q from 0 to (p-1)*q = 2044, zeros elsewhere. */
    int num[2045];
    for (int i = 0; i <= 2044; ++i) num[i] = 0;
    for (int i = 0; i <= p - 1; ++i) num[i * q] = 1;

    /* Long division: num = den * coeff.  All divisions are exact. */
    for (int i = 2044; i >= 28; --i) {
        int c = num[i];                   /* leading coefficient of the current remainder */
        int pos = i - 28;                 /* position in coeff[] */
        coeff[pos] = c;                   /* quotient coefficient */

        /* Subtract c * den(x) from the proper segment of num[] */
        if (c != 0) {
            for (int j = 0; j < 29; ++j) {
                num[pos + j] -= c * den[j];
            }
        }
    }
    /* The remainder should be zero.  coeff[0..DEG] now holds Φ_2117 coefficients. */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* -----------------------------------------------------------------
       Static cache for the period of length 2117.  The cache is
       initialized on the first call.  All indices refer to the
       periodic sequence of inverse cyclotomic polynomial coefficients.
       ----------------------------------------------------------------- */
    static bool initialized = false;
    static seq_t cache[PERIOD];
    if (!initialized) {
        /* ---- compute Φ_2117 coefficients -------------------------------- */
        int phi[DEG + 1];
        compute_phi_coeffs(phi);

        /* ---- compute inverse series coefficients up to period-1 ------- */
        cache[0] = (seq_t)1;
        for (int nidx = 1; nidx < PERIOD; ++nidx) {
            int sum = 0;
            int maxk = nidx <= DEG ? nidx : DEG;
            for (int k = 1; k <= maxk; ++k) {
                sum += phi[k] * (int)cache[nidx - k];
            }
            cache[nidx] = (seq_t)(-sum);
        }
        initialized = true;
    }

    /* Return the periodic coefficient. */
    return cache[n % PERIOD];
}
