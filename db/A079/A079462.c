/*
 * A079462  Nonzero coefficients of the polynomials in the numerator of the
 *          generating function x/(1-x-x^2) for the Fibonacci sequence and its
 *          successive derivatives starting with the constant.
 * https://oeis.org/A079462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less, tabl
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A079462 – nonzero coefficients of the numerators of the
   successive derivatives of x/(1-x-x^2), starting with the constant.
   Implementation: iterate the recurrence
        N_{k+1} = D * N_k'  -  (k+1) * D' * N_k         (*)
   where D = 1 - x - x², D' = -1 - 2x and N₀ = x.
   The k‑th polynomial N_k has k+1 non‑zero terms; its coefficients are
   multiplied by (-1)^{k+1} for k ≥ 1.  The sequence is the list of
   all non‑zero coefficients of:
        1  (constant term)
        N₁ (positive sign)
        N₂ (negative sign)
        N₃ (positive sign)
        …
   The function is self‑contained and uses only integral types.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 (constant polynomial) */
    if (n == 0)
        return (seq_t)1;

    /* Convert index to a zero‑based index among derivative coefficients */
    idx_t m = n - 1;                 /* m ≥ 0 */

    /* Find the derivative order k such that the first m+1 terms fall into
       the cumulative count up to k.  The count of terms contributed by
       derivative k is (k+1).  The cumulative count up to k is
           C(k) = (k+1)(k+2)/2
       We search for the smallest k ≥ 1 with C(k) > m. */
    idx_t k = 0;
    idx_t cumulative = 0;
    while (1) {
        ++k;
        cumulative += (k + 1);       /* add terms of derivative k */
        if (m < cumulative)
            break;
    }

    /* The index of the desired non‑zero coefficient within N_k */
    idx_t prev_cumulative = cumulative - (k + 1);   /* C(k-1) */
    idx_t pos = m - prev_cumulative;               /* 0‑based */

    /* Compute the polynomial N_k using the recurrence (*).
       We start from N₀(x) = x   →  [0, 1] (degree 1).  */
    idx_t prevSize = 2;                      /* length of N₀ */
    seq_t *prev = (seq_t *)malloc(prevSize * sizeof(seq_t));
    if (!prev) return (seq_t)0;              /* out of memory */

    prev[0] = (seq_t)0;
    prev[1] = (seq_t)1;

    /* Build up to N_k */
    for (idx_t cur = 1; cur <= k; ++cur) {
        /* Derivative of current polynomial: N' */
        idx_t derivSize = prevSize - 1;
        seq_t *deriv = (seq_t *)malloc(derivSize * sizeof(seq_t));
        for (idx_t i = 0; i < derivSize; ++i)
            deriv[i] = (seq_t)(i + 1) * prev[i + 1];   /* (i+1) * coeff(i+1) */

        /* New polynomial size: current size + 1 */
        idx_t newSize = prevSize + 1;
        seq_t *newN = (seq_t *)calloc(newSize, sizeof(seq_t));
        seq_t factor = (seq_t)cur;                    /* k in the recurrence */

        for (idx_t e = 0; e < newSize; ++e) {
            seq_t val = (seq_t)0;

            /* D * N'  (D = 1 - x - x²) */
            if (e < derivSize)                    val += deriv[e];
            if (e >= 1 && e - 1 < derivSize)      val -= deriv[e - 1];
            if (e >= 2 && e - 2 < derivSize)      val -= deriv[e - 2];

            /* + factor * (1 + 2x) * N   (since -(k)*D' = k*(1+2x)) */
            if (e < prevSize)                     val += factor * prev[e];
            if (e >= 1 && e - 1 < prevSize)       val += (seq_t)(2 * factor) * prev[e - 1];

            newN[e] = val;
        }

        free(prev);
        free(deriv);
        prev = newN;
        prevSize = newSize;
    }

    /* Extract the pos‑th non‑zero coefficient of N_k */
    seq_t coeff = 0;
    idx_t seen = 0;
    for (idx_t e = 0; e < prevSize; ++e) {
        if (prev[e] == 0) continue;
        if (seen == pos) {
            coeff = prev[e];
            break;
        }
        ++seen;
    }
    free(prev);

    /* Apply the sign factor (-1)^{k+1} for k ≥ 1.  For k = 0 the factor
       is irrelevant (we already handled n = 0 above). */
    seq_t sign = ((k + 1) & 1) ? (seq_t)-1 : (seq_t)1;

    return sign * coeff;
}
