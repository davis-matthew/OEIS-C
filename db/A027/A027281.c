/*
 * A027281  a(n) = Sum_{k=0..2n} (k+1) * A026568(n, k).
 * https://oeis.org/A027281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2n} (k+1) * A026568(n,k) Generating function:
 *            (2x^2 + 4x + 1) / (1 - x - 4x^2)^2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static tables: q    – coefficients of 1/(1 - x - 4x^2)
                        h2   – coefficients of 1/(1 - x - 4x^2)^2
                        aArr – target sequence
       'size' is the currently allocated length (number of stored indices)   */
    static seq_t *q   = NULL;
    static seq_t *h2  = NULL;
    static seq_t *aArr = NULL;
    static idx_t size  = 0;   /* number of elements allocated (>= 0) */

    /* grow (if necessary) */
    if (n + 1 > size) {
        idx_t newSize = n + 1;
        seq_t *tmp;

        tmp = (seq_t *)realloc(q,   newSize * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        q = tmp;

        tmp = (seq_t *)realloc(h2,  newSize * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        h2 = tmp;

        tmp = (seq_t *)realloc(aArr, newSize * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        aArr = tmp;

        /* initialise from the old end up to the new limit */
        for (idx_t i = size; i <= n; ++i) {
            /* q(i) = q(i-1) + 4 * q(i-2), initial q(0)=q(1)=1 */
            if (i == 0) {
                q[i] = (seq_t)1;
            } else if (i == 1) {
                q[i] = (seq_t)1;
            } else {
                q[i] = q[i-1] + (q[i-2] * (seq_t)4);
            }

            /* h2(i) = h2(i-1) + 4 * h2(i-2) + q(i),    h2(0)=1, h2(1)=2 */
            if (i == 0) {
                h2[i] = (seq_t)1;
            } else if (i == 1) {
                h2[i] = (seq_t)2;   /* 1 (h2[0]) + 0 + q[1] (1) = 2 */
            } else {
                h2[i] = h2[i-1] + (h2[i-2] * (seq_t)4) + q[i];
            }

            /* a(i) = h2(i) + 4*h2(i-1) + 2*h2(i-2) with missing terms as 0 */
            if (i == 0) {
                aArr[i] = h2[i];
            } else if (i == 1) {
                aArr[i] = h2[i] + (h2[i-1] * (seq_t)4);
            } else {
                aArr[i] = h2[i]
                        + (h2[i-1] * (seq_t)4)
                        + (h2[i-2] * (seq_t)2);
            }
        }
        size = newSize;
    }

    return aArr[n];
}
