/*
 * A078997  Nonzero coefficients of the polynomials in the denominator of the
 *          generating function x/(1-x-x^2) for the Fibonacci sequence and its
 *          successive derivatives starting with the highest power of x.
 * https://oeis.org/A078997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   a(n) = coefficients (ignoring zeros) of the polynomial (1 - x -
 *            x^2)^(k) with k = n+1, listed from highest degree to constant
 *            term. For odd k > 1 all coefficients are multiplied by -1
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial polynomial for k = 1: (1 - x - x^2) */
    size_t curLen = 3;
    seq_t *cur = (seq_t *)malloc(curLen * sizeof(seq_t));
    cur[0] = (seq_t)1;
    cur[1] = (seq_t)-1;
    cur[2] = (seq_t)-1;

    idx_t count = 0;   /* Number of terms already yielded */

    for (idx_t k = 1;; ++k) {
        /* Sign adjustment: flip all coefficients if k > 1 and k is odd */
        seq_t sign = 1;
        if (k > 1 && (k % 2 == 1))
            sign = (seq_t)-1;

        size_t degMax = 2 * k;
        /* Emit non‑zero coefficients of cur in descending degree order */
        for (size_t d = degMax; ; --d) {
            seq_t coeff = cur[d];
            if (coeff != (seq_t)0) {
                if (count == n) {
                    seq_t result = sign * coeff;
                    free(cur);
                    return result;
                }
                ++count;
            }
            if (d == 0)
                break;
        }

        /* Build next polynomial = cur * (1 - x - x^2) */
        size_t nextLen = 2 * (k + 1) + 1;
        seq_t *next = (seq_t *)calloc(nextLen, sizeof(seq_t));
        for (size_t i = 0; i < curLen; ++i) {
            seq_t v = cur[i];
            if (v != (seq_t)0) {
                next[i + 0] += v;          /* * 1  */
                next[i + 1] -= v;          /* * -1 */
                next[i + 2] -= v;          /* * -1 */
            }
        }
        free(cur);
        cur = next;
        curLen = nextLen;
    }
}
