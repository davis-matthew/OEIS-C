/*
 * A009443  Expansion of log(1+x/cosh(x)).
 * https://oeis.org/A009443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficients of log(1 + x/cosh(x)) in the ordinary power
 *            series (OEIS A009443). The recurrence derived below uses only
 *            integer arithmetic. The sequences are computed for all indices
 *            up to the largest requested value; repeated calls are handled
 *            efficiently
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 15, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
seq_t a(idx_t n)
{
    static idx_t maxN = 0;      /* largest n for which seq and pow2 are built */
    static seq_t *seq = NULL;   /* seq[i] = a(i) */
    static seq_t *pow2 = NULL;  /* pow2[i] = 2^i   (for use with even k) */
 
    /* Allocate / grow the tables if necessary */
    if (seq == NULL || n > maxN) {
        /* new required size */
        size_t newSize = (size_t)(n + 1);          /* indices 0 .. n */
        seq  = (seq_t *)realloc(seq,  newSize * sizeof(seq_t));
        pow2 = (seq_t *)realloc(pow2, newSize * sizeof(seq_t));
        if (seq == NULL || pow2 == NULL)
            exit(EXIT_FAILURE);                   /* allocation failure */
 
        /* base values */
        pow2[0] = (seq_t)1;
        seq[0] = (seq_t)0;
        if (n >= 1) seq[1] = (seq_t)1;
 
        /* compute all powers of two up to n-1 (needed for R_k) */
        for (idx_t i = 1; i <= n; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;
 
        /* compute a(m) for all m > maxN and <= n */
        for (idx_t m = maxN + 1; m <= n; ++m) {
            if (m == 0) { seq[0] = (seq_t)0; continue; }
            if (m == 1) { seq[1] = (seq_t)1; continue; }
 
            seq_t sum = 0;
            seq_t binom = (seq_t)1;                /* C(m-1,0) */
 
            for (idx_t k = 1; k <= m - 1; ++k) {
                /* binom := C(m-1, k) */
                binom = binom * (seq_t)(m - k) / (seq_t)k;
 
                seq_t Rk = (k & 1) ? (seq_t)k : pow2[k - 1];  /* R_k */
                seq_t term = binom * Rk * seq[m - k];
                sum += term;
            }
 
            if ((m & 1) == 0)                     /* m even */
                seq[m] = -sum;
            else                                  /* m odd  */
                seq[m] = (seq_t)(2 - (int) m) - sum;
        }
        maxN = n;
    }
    return seq[n];
}
