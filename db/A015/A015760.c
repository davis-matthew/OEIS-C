/*
 * A015760  Inverse of 1751st cyclotomic polynomial.
 * https://oeis.org/A015760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the power‑series expansion of
 *            1/Φ_{1751}(x), periodic with period 1751
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine builds Φ_{1751}(x) by polynomial division and then uses the
   recurrence sum_{j=0}^{φ(1751)} φ_j · a(n−j) = δ_{n,0} to generate the
   periodic sequence. */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* constants for n = 1751 = 17 × 103 */
    const int p = 17;
    const int q = 103;
    const int nmax = p * q;                 /* 1751 */
    const int degPhi = (p - 1) * (q - 1);   /* φ(1751) = 1632 */
    const int degD = p + q - 1;             /* degree of denominator (x^p‑1)(x^q‑1)/(x‑1) = 119 */
    const int degN  = nmax;                 /* degree of numerator x^n – 1 = 1751 */

    static bool ready = false;
    static seq_t seq[1751];                 /* periodic values a(0)…a(1749)  */
    static seq_t phi[1633];                 /* coefficients of Φ_{1751}(x) : phi[0]…phi[1632] */

    if (!ready) {
        /* ----- Build denominator D(x) = (x^p‑1)(x^q‑1)/(x‑1) ----- */
        seq_t r[119] = {0};                 /* convolution of 1+x+…+x^{p‑1} and 1+x+…+x^{q‑1} */
        for (int i = 0; i < p; ++i)
            for (int j = 0; j < q; ++j)
                ++r[i + j];

        seq_t d[120] = {0};
        for (int k = 0; k <= 119; ++k) {
            seq_t prev = (k == 0) ? 0 : r[k - 1];
            seq_t curr = (k <= 118) ? r[k] : 0;
            d[k] = prev - curr;              /* d[0] = -1, d[119] = +1, others as needed */
        }

        /* ----- Build numerator N(x) = x^nmax – 1 ----- */
        seq_t C[1752] = {0};                 /* coefficients of current dividend */
        C[degN] = 1;                         /* leading term x^nmax */
        C[0]    = -1;                        /* constant term –1 */

        /* ----- Polynomial long division: N(x) / D(x) = Φ_{1751}(x) ----- */
        for (int i = degN; i >= degD; --i) {
            if (C[i] != 0) {
                const seq_t coeff = C[i];
                phi[i - degD] = coeff;        /* coefficient of x^{i‑degD} in Φ */
                /* subtract coeff * D(x) shifted by (i‑degD) from C */
                int shift = i - degD;
                for (int k = 0; k <= 119; ++k)
                    C[shift + k] -= coeff * d[k];
            }
        }
        /* now phi[0]..phi[1632] hold the coefficients of Φ_{1751}(x) */

        /* ----- Recurrence to generate a(n) for one period ----- */
        seq[0] = 1;                         /* a(0) = 1 */
        for (int k = 1; k < 1751; ++k) {
            seq_t sum = 0;
            int maxj = (k < degPhi) ? k : degPhi;   /* sum over j = 1 … min(k,φ(1751)) */
            for (int j = 1; j <= maxj; ++j)
                sum += phi[j] * seq[k - j];
            seq[k] = -sum;                /* because φ_0 = 1 */
        }

        ready = true;
    }

    return seq[n % 1751];
}
