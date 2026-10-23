/*
 * A168741  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, and for n ≥ 1 * a(n) = Σ_{k=1}^{17} 14·a(n-k) * – (n ≥
 *            18 ? 105·a(n-18) : 0) * + p[n] , * where p[0] = 1, * p[1] …
 *            p[17] = 2, * p[18] = 1, * p[n] = 0 for n > 18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated from the rational function
 * G(t) = (t¹⁸ + 2t¹⁷ + 2t¹⁶ + … + 2t + 1) /
 *        (1 – 14t – 14t² – … – 14t¹⁷ + 105t¹⁸)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;    /* cache of computed values */
    static idx_t buflen = 0;     /* number of stored values */

    /* extend cache if necessary */
    if (n + 1 > buflen) {
        idx_t oldlen = buflen;
        idx_t newlen = n + 1;

        /* reallocate buffer */
        seq_t *tmp = NULL;
#ifdef __cplusplus
        tmp = new seq_t[newlen];
#else
        tmp = (seq_t *)malloc(newlen * sizeof(seq_t));
#endif
        if (!tmp) return 0;          /* allocation failed – undefined */

        /* copy existing entries */
        for (idx_t i = 0; i < oldlen; ++i)
            tmp[i] = buf[i];

        /* free old buffer */
        if (buf) {
#ifdef __cplusplus
            delete [] buf;
#else
            free(buf);
#endif
        }
        buf = tmp;
        buflen = newlen;

        /* compute new terms */
        for (idx_t i = oldlen; i <= n; ++i) {
            seq_t sum = 0;

            /* 14 times the last 17 terms */
            for (idx_t k = 1; k <= 17 && i >= k; ++k)
                sum += (seq_t)14 * buf[i - k];

            /* minus 105 times a(n-18) if available */
            if (i >= 18)
                sum -= (seq_t)105 * buf[i - 18];

            /* add the polynomial term p[n] */
            if (i == 0)
                sum += 1;
            else if (i == 18)
                sum += 1;
            else if (i >= 1 && i <= 17)
                sum += 2;
            /* otherwise p[n] = 0 */

            buf[i] = sum;
        }
    }
    return buf[n];
}
