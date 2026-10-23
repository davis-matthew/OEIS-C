/*
 * A014658  Inverse of 649th cyclotomic polynomial.
 * https://oeis.org/A014658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of the inverse of the 649‑th cyclotomic
 *            polynomial Φ_649(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The sequence has period 649, so we compute the period once and reuse it. */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t R[581];   /* coefficients of Φ_649(x), degree 580 */
    static seq_t A[649];   /* coefficients of the inverse series */

    if (!init) {
        /* Compute Φ_649(x) via the relation
           Φ_649(x) = (x-1)(x^649-1) / ((x^11-1)(x^59-1)).          */
        const seq_t P[651] = {0};  /* temporary storage for P_n if needed */

        /* We'll compute R in place using the recurrence
           R[n] - R[n-11] - R[n-59] + R[n-70] = P_n,
           where P_n = 1 for n=0, -1 for n=1, 0 otherwise up to n=580.  */
        for (idx_t i = 0; i <= 580; ++i) R[i] = 0;

        for (idx_t i = 0; i <= 580; ++i) {
            seq_t Pn = 0;
            if (i == 0) Pn = 1;
            else if (i == 1) Pn = -1;

            seq_t val = Pn;
            if (i >= 11) val += R[i - 11];
            if (i >= 59) val += R[i - 59];
            if (i >= 70) val -= R[i - 70];
            R[i] = val;
        }

        /* Compute the inverse series coefficients A[n] via
           a[0] = 1,
           a[n] = -∑_{k=1}^{min(580,n)} R[k] * a[n-k]  for n >= 1.       */
        A[0] = 1;
        for (idx_t nidx = 1; nidx < 649; ++nidx) {
            seq_t sum = 0;
            idx_t upper = (nidx < 580) ? nidx : 580;
            for (idx_t k = 1; k <= upper; ++k) {
                if (R[k] != 0)
                    sum += R[k] * A[nidx - k];
            }
            A[nidx] = -sum;
        }

        init = true;
    }

    return A[n % 649];
}
