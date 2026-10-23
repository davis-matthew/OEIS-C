/*
 * A016336  145th cyclotomic polynomial.
 * https://oeis.org/A016336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) is the coefficient of x^n in the 145th cyclotomic
 *            polynomial Φ_145(x)
 * Range:     i8 n <= 112, i16 n <= 112, i32 n <= 112, i64 n <= 112,
 *            i128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Φ_145(x) = Φ_5(x^29)/Φ_5(x)  (p = 5, q = 29). */
    /* The coefficient sequence can be computed by the recurrence */
    /*   q_k + q_{k-1} + q_{k-2} + q_{k-3} + q_{k-4} = N_k,      */
    /*   where N_k = 1 if k ≡ 0 (mod 29), otherwise 0,      */
    /*   and q_k = a(k).                                 */
    /* Degree of Φ_145 is (5-1)(29-1) = 112.             */

    static bool initialized = false;
    static seq_t coeff[113];           /* indices 0 .. 112 */
    if (!initialized) {
        const idx_t p = 5, q = 29;
        const idx_t deg = (p - 1) * (q - 1);   /* 112 */

        for (idx_t k = 0; k <= deg; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i < p; ++i) {
                if (k >= i) {
                    sum += coeff[k - i];
                }
            }
            seq_t N = (k % q == 0) ? (seq_t)1 : (seq_t)0;
            coeff[k] = N - sum;    /* q_k = N_k - sum(q_{k-1..k-(p-1)}) */
        }
        initialized = true;
    }

    if (n <= 112u) {
        return coeff[n];
    }
    return (seq_t)0;
}
