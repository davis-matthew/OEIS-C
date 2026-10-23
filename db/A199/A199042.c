/*
 * A199042  Expansion of e.g.f. exp(arctan(1+2*x) - Pi/4).
 * https://oeis.org/A199042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=1,
 * a(n+1)=-(2n-1)*a(n) - 2*n*(n-1)*a(n-1)   for n≥1
 * (derived from (1+2x+2x²)f' = f for f(x)=exp(arctan(1+2x)-π/4))
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static idx_t      size = 0;   /* number of pre‑computed terms   */
    static seq_t *    A    = NULL;/* array of values a(0)..a(size‑1)*/

    /* initialise the first two terms if needed */
    if (size == 0) {
        size = 2;
        A = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!A) return (seq_t)0;  /* fallback on allocation failure */
        A[0] = (seq_t)1;
        A[1] = (seq_t)1;
    }

    /* extend array until it contains a(n) */
    if (n >= size) {
        A = (seq_t*)realloc(A, (n + 1) * sizeof(seq_t));
        if (!A) return (seq_t)0;  /* fallback on allocation failure */
        for (idx_t i = size; i <= n; ++i) {
            /* recurrence applies only for i≥2 */
            if (i == 1) {               /* already set during init */
                continue;
            }
            idx_t k = i - 1;            /* k = n in the recurrence */
            seq_t term1 = (seq_t)(2 * k - 1) * A[k];
            seq_t term2 = (seq_t)2 * (seq_t)k * (seq_t)(k - 1) * A[k - 1];
            A[i] = -(term1 + term2);
        }
        size = n + 1;
    }

    return A[n];
}
