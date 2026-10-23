/*
 * A144180  Number of ways of placing n labeled balls into n unlabeled (but
 *          5-colored) boxes.
 * https://oeis.org/A144180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 * Σ_{k=0}^{n-1} C(n-1,k) * a(k); a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised array of a(k) for k = 0 .. max_n */
    static seq_t *A = NULL;   /* array of values */
    static idx_t max_n = 0;    /* biggest index computed */

    /* first call – allocate space for a(0) */
    if (A == NULL) {
        A = (seq_t *)malloc(sizeof(seq_t));
        if (!A) return (seq_t)0;   /* out of memory – unlikely in tests */
        A[0] = (seq_t)1;           /* a(0) = 1 */
    }

    /* if the requested n is larger than what we have, extend the table */
    if (n > max_n) {
        seq_t *tmp = (seq_t *)realloc(A, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* realloc failed – unlikely */

        A = tmp;

        /* compute a(max_n+1) .. a(n) using the binomial‑transform
         * recurrence: a(m) = 5 * Σ_{k=0}^{m-1} C(m-1,k) * a(k)   (m >= 1)
         */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            idx_t prev   = m - 1;            /* n-1 for the binomial coeffs   */
            seq_t sum    = (seq_t)0;        /* accumulator for the sum        */
            seq_t binom  = (seq_t)1;        /* C(prev,0) = 1                  */

            for (idx_t k = 0; k <= prev; ++k) {
                sum += binom * A[k];                     /* add term */
                if (k < prev) {                          /* next binomial */
                    binom = binom * (seq_t)(prev - k) / (seq_t)(k + 1);
                }
            }
            A[m] = (seq_t)5 * sum;                      /* recurrence */
        }
        max_n = n;
    }

    return A[n];
}
