/*
 * A101195  Expansion of psi(x^3) / psi(x) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A101195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k + x^{2k})*(1 - x^k
 *            + x^{2k})^2
 * Range:     i8 n <= 25, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* work array size */
    size_t len = (size_t)(n + 1);

    /* allocate two working arrays of seq_t */
    seq_t *cur  = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(len, sizeof(seq_t));
    if (!cur || !next) {
        free(cur);
        free(next);
        return (seq_t)0;          /* allocation failure – should not happen */
    }

    cur[0] = (seq_t)1;           /* initial product = 1 */

    /* coefficients of (1 + x^k + x^{2k})*(1 - x^k + x^{2k})^2
       = 1 - x^k + 2x^{2k} - x^{3k} + 2x^{4k} - x^{5k} + x^{6k} */
    static const seq_t coeff[7] = {
        (seq_t) 1, (seq_t)-1, (seq_t) 2, (seq_t)-1,
        (seq_t) 2, (seq_t)-1, (seq_t) 1
    };

    for (idx_t k = 1; k <= n; ++k) {
        /* clear next array */
        for (size_t i = 0; i < len; ++i)
            next[i] = (seq_t)0;

        /* apply the 7 terms of the polynomial for this k */
        for (int m = 0; m < 7; ++m) {
            size_t shift;
            if (m == 0) {
                shift = 0;
            } else {
                /* avoid overflow of k*m */
                if (k > n / (idx_t)m)
                    break;
                shift = (size_t)(k * (idx_t)m);
            }

            seq_t c = coeff[m];
            size_t limit = len - 1 - shift;  /* i + shift <= n */
            for (size_t i = 0; i <= limit; ++i)
                next[i + shift] += cur[i] * c;
        }

        /* swap cur and next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[(size_t)n];

    free(cur);
    free(next);
    return result;
}
