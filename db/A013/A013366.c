/*
 * A013366  cosh(sin(x)-arctan(x))=1+10/6!*x^6-1288/8!*x^8+152934/10!*x^10...
 * https://oeis.org/A013366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^(2n) in cosh(sin(x)-atan(x)), multiplied
 *            by (2n)! (OEIS A013366)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute a(idx_t n) given the types seq_t and idx_t defined by the build */
seq_t a(idx_t n)
{
    /* we need terms up to degree 2n */
    idx_t maxDeg = 2 * n;

    /* Allocate dynamic arrays */
    seq_t *E     = (seq_t*)malloc((maxDeg + 1) * sizeof(seq_t));
    seq_t *Eneg  = (seq_t*)malloc((maxDeg + 1) * sizeof(seq_t));
    seq_t *M     = (seq_t*)malloc((maxDeg + 1) * sizeof(seq_t));
    if (!E || !Eneg || !M) return 0;          /* safety – should never happen */

    /* Pre‑compute the integers M[k] = k! * c_k, where
       c_k = coefficient of x^k in sin(x)-atan(x)  (k odd).   */
    seq_t fact = 1;                          /* (i-1)!  */
    for (idx_t i = 1; i <= maxDeg; ++i) {
        if ((i & 1)) {                     /* odd i */
            idx_t m  = (i - 1) >> 1;       /* m = (i-1)/2 */
            seq_t sign = (m & 1) ? (seq_t)(-1) : (seq_t)1;
            /* 1 - fact  = 1 - (i-1)!  */
            M[i] = sign * (seq_t)(1 - fact);
        } else {
            M[i] = 0;
        }
        fact = fact * (seq_t)i;  /* fact becomes i! for next loop */
    }

    /* Initialisation of the exponential series */
    E[0]    = 1;
    Eneg[0] = 1;                     /* e^{g} and e^{−g} start with 1 */

    /* Main recurrence: E[n] = Σ C(n-1,k-1) * M[k] * E[n-k]   */
    for (idx_t len = 1; len <= maxDeg; ++len) {
        seq_t sum    = 0;
        seq_t sumNeg = 0;
        seq_t binom  = 1;            /* C(len-1,0) */

        for (idx_t k = 1; k <= len; ++k) {
            if (M[k] != 0) {
                /* Term for E */
                seq_t term = binom * M[k];
                term = term * E[len - k];
                sum += term;

                /* Term for Eneg (uses -M[k], i.e. negative sign) */
                seq_t termNeg = binom * M[k];
                termNeg = termNeg * Eneg[len - k];
                termNeg = -termNeg;
                sumNeg += termNeg;
            }

            if (k < len) {
                /* Update binomial coefficient:
                   C(len-1, k) = C(len-1, k-1) * (len-k)/k  */
                binom = binom * (seq_t)(len - k);
                binom = binom / (seq_t)k;
            }
        }

        E[len]    = sum;
        Eneg[len] = sumNeg;
    }

    /* Result: a(n) = (E[2n] + Eneg[2n]) / 2   */
    seq_t result = (E[2 * n] + Eneg[2 * n]) / (seq_t)2;

    /* Free allocated memory */
    free(E);
    free(Eneg);
    free(M);

    return result;
}
