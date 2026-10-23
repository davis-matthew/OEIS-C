/*
 * A014054  Inverse of 45th cyclotomic polynomial.
 * https://oeis.org/A014054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series
 *            expansion of 1/Φ_45(x), where Φ_45 is the 45th cyclotomic
 *            polynomial. The sequence is periodic with period 45, so the
 *            value for any n≥0 can be obtained from the first 45 terms
 *            computed once
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   multiply_polynomials
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/multiply_polynomials.h"

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* First 45 terms are precomputed and stored in this static table. */
    static seq_t table[45];
    static bool ready = false;

    if (!ready) {
        /* Define known cyclotomic polynomials. */
        const seq_t phi1[2] = {-1, 1};                          /* x - 1            */
        const seq_t phi3[3] = {1, 1, 1};                      /* x^2 + x + 1      */
        const seq_t phi5[5] = {1, 1, 1, 1, 1};                /* x^4 + x^3 + x^2 + x + 1 */
        const seq_t phi9[7] = {1, 0, 0, 1, 0, 0, 1};          /* x^6 + x^3 + 1    */
        const seq_t phi15[9] = {1, -1, 0, 1, -1, 1, 0, -1, 1};/* x^8 - x^7 + x^5 - x^4 + x^3 - x + 1 */

        /* Multiply the above polynomials gradually to obtain
           P(x) = Φ_1 Φ_3 Φ_5 Φ_9 Φ_15.              */
        seq_t t1[4];
        seq_t t2[8];
        seq_t t3[14];
        seq_t P[22];   /* degree 21 */

        multiply_polynomials(phi1, 1, phi3, 2, t1);       /* degrees 0..3 */
        multiply_polynomials(t1, 3, phi5, 4, t2);         /* degrees 0..7 */
        multiply_polynomials(t2, 7, phi9, 6, t3);         /* degrees 0..13 */
        multiply_polynomials(t3, 13, phi15, 8, P);       /* degrees 0..21 */

        /* Perform the exact division: (x^45 - 1) / P(x) = Φ_45(x). */
        seq_t num[46];
        seq_t Q[25];  /* Φ_45 has degree 24 */

        for (size_t i = 0; i < 46; ++i) num[i] = 0;
        num[0] = -1;
        num[45] = 1;   /* x^45 - 1 */

        /* Long division.  The leading coefficient of P(x) is 1. */
        for (int i = 45; i >= 21; --i) {
            seq_t qcoef = num[i];
            Q[i - 21] = qcoef;
            for (int j = 0; j <= 21; ++j)
                num[(i - 21) + j] -= qcoef * P[j];
        }
        /* Q[0] should be 1 (constant term). */

        /* Now compute the first 45 terms of the sequence
           using the recurrence a[n] = -∑_{k=1}^{min(n,24)} Φ_45_k * a[n-k]. */
        table[0] = 1;
        for (size_t n = 1; n < 45; ++n) {
            seq_t sum = 0;
            size_t maxk = (n < 24 ? n : 24);
            for (size_t k = 1; k <= maxk; ++k)
                sum += Q[k] * table[n - k];
            table[n] = -sum;
        }

        ready = true;
    }

    return table[n % 45];
}
