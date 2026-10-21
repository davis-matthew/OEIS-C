/*
 * A063031  Reversion of y - y^2 - y^3 - y^4 + y^5.
 * https://oeis.org/A063031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reversion of y - y^2 - y^3 - y^4 + y^5
   Series reversion by iterative convolution:
   a(0)=0, a(1)=1
   For n ≥ 2:
       a[n] =   conv2[n] + conv3[n] + conv4[n] - conv5[n]
   where convk[n] = coefficient of yⁿ in h(y)^k,
   h(y) = Σ_{i≥1} a[i] y^i.  The values convk are obtained from
   previously computed a[1…n‑1] by recurrence.
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;

    /* Allocate arrays for the sequence and the convolution results. */
    seq_t *a_val  = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *c2     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *c3     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *c4     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *c5     = (seq_t*)calloc(N + 1, sizeof(seq_t));

    /* Initial values */
    a_val[1] = (seq_t)1;

    for (size_t m = 2; m <= N; ++m) {
        seq_t s2 = 0, s3 = 0, s4 = 0, s5 = 0;

        /* conv2[m] = Σ_{i=1}^{m-1} a[i]·a[m-i] */
        for (size_t i = 1; i < m; ++i) {
            s2 += a_val[i] * a_val[m - i];
        }
        c2[m] = s2;

        /* conv3[m] = Σ_{p=1}^{m-2} a[p]·conv2[m-p]  (if m≥3) */
        if (m >= 3) {
            for (size_t p = 1; p <= m - 2; ++p) {
                s3 += a_val[p] * c2[m - p];
            }
            c3[m] = s3;
        }

        /* conv4[m] = Σ_{p=1}^{m-3} a[p]·conv3[m-p]  (if m≥4) */
        if (m >= 4) {
            for (size_t p = 1; p <= m - 3; ++p) {
                s4 += a_val[p] * c3[m - p];
            }
            c4[m] = s4;
        }

        /* conv5[m] = Σ_{p=1}^{m-4} a[p]·conv4[m-p]  (if m≥5) */
        if (m >= 5) {
            for (size_t p = 1; p <= m - 4; ++p) {
                s5 += a_val[p] * c4[m - p];
            }
            c5[m] = s5;
        }

        /* a[m] = conv2 + conv3 + conv4 - conv5 */
        a_val[m] = c2[m] + c3[m] + c4[m] - c5[m];
    }

    seq_t result = a_val[N];

    /* Clean up */
    free(a_val);
    free(c2);
    free(c3);
    free(c4);
    free(c5);

    return result;
}
