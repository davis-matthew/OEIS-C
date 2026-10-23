/*
 * A016384  329th cyclotomic polynomial.
 * https://oeis.org/A016384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 329th cyclotomic polynomial
 *            Φ_329(x). The polynomial has degree φ(329)=276; for n>276 the
 *            value is 0. It is computed once by dividing Φ_7(x^47) by Φ_7(x)
 * Range:     i8 n <= 276, i16 n <= 276, i32 n <= 276, i64 n <= 276,
 *            i128 n <= 276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 329 = 7·47  =>  p=7, q=47, degree = (p-1)(q-1)=276 */
    const idx_t p = 7;
    const idx_t q = 47;
    const idx_t degQ = (p - 1) * (q - 1);   /* 276 */

    static seq_t coeffs[277];   /* indices 0..degQ */
    static bool init = false;

    if (!init) {
        /* Compute Q(x) = Φ_p(x^q) / Φ_p(x) by a linear recurrence.
           For each k (0 ≤ k ≤ degQ):
           Q_k = A_k - Σ_{j=1}^{p-1} Q_{k-j},
           where A_k = 1 if k is a multiple of q and k/q ≤ p-1, else 0. */
        for (idx_t k = 0; k <= degQ; ++k) {
            seq_t neg_sum = (seq_t)0;
            for (idx_t j = 1; j < p; ++j) {
                if (k >= j) {
                    neg_sum -= coeffs[k - j];
                }
            }
            seq_t A_k = (k % q == 0 && (k / q) <= (p - 1)) ? (seq_t)1 : (seq_t)0;
            coeffs[k] = A_k + neg_sum;   /* Q_k = A_k - Σ Q_{k-j} */
        }
        init = true;
    }

    if (n > degQ) {
        return (seq_t)0;
    }
    return coeffs[n];
}
