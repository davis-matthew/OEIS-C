/*
 * A009593  Expansion of e.g.f. sinh(sin(x)*cos(x)), odd powers only.
 * https://oeis.org/A009593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (d(2n+1) – d₂(2n+1)) / 2 where d(m) and d₂(m) satisfy
 *            d(0) = d₂(0) = 1 d(m) = Σ_{k odd ≤ m} (-1)^{(k-1)/2} 4^{(k-1)/2}
 *            * C(m-1, k-1) * d(m-k) d₂(m) = Σ_{k odd ≤ m} -(-1)^{(k-1)/2}
 *            4^{(k-1)/2} * C(m-1, k-1) * d₂(m-k) (C denotes binomial
 *            coefficients). The recurrence uses only integer operat...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* needed maximum index (2n+1) */
    if (n > (idx_t)(-1) / 2) {              /* overflow guard */
        return (seq_t)0;
    }
    idx_t M = 2 * n + 1;                    /* maximum m to compute */

    /* static storage for the two sequences */
    static seq_t *d  = NULL;                /* d(m)   */
    static seq_t *d2 = NULL;                /* d₂(m)  */
    static idx_t cap = 0;                   /* current capacity */
    static idx_t comp = 0;                  /* largest m already computed */

    /* enlarge arrays if necessary */
    if (M + 1 > cap) {
        idx_t new_cap = (cap == 0) ? 8 : cap * 2;
        if (new_cap < M + 1) new_cap = M + 1;

        seq_t *dnew  = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        seq_t *d2new = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!dnew || !d2new) {
            /* allocation failed – free any partially allocated memory */
            if (dnew)  free(dnew);
            if (d2new) free(d2new);
            return (seq_t)0;
        }

        if (d)   memcpy(dnew,   d,   sizeof(seq_t) * cap);
        if (d2)  memcpy(d2new, d2,  sizeof(seq_t) * cap);
        if (d)   free(d);
        if (d2)  free(d2);

        d   = dnew;
        d2  = d2new;
        cap = new_cap;
    }

    /* initialise at m=0 */
    if (comp == 0) {
        d[0]  = (seq_t)1;
        d2[0] = (seq_t)1;
    }

    /* compute values from comp+1 up to M */
    for (idx_t m = comp + 1; m <= M; ++m) {
        seq_t sum_d  = (seq_t)0;
        seq_t sum_d2 = (seq_t)0;

        idx_t n_minus_1 = m - 1;
        idx_t r = 0;                       /* r = k-1 for current k */
        seq_t binom = (seq_t)1;            /* C(n_minus_1, r) */
        int    sign = 1;                   /* sign of B_int(k) = (-1)^{(k-1)/2} 4^{(k-1)/2} */
        seq_t pow4 = (seq_t)1;             /* 4^{(k-1)/2} */

        /* iterate over odd k */
        for (idx_t k = 1; k <= m; k += 2) {
            idx_t idx = m - k;             /* index for d[idx] and d2[idx] */

            seq_t mult_common = binom * pow4;        /* |B_int(k)| * C(n-1, k-1) */

            /* contribution to d(m) */
            seq_t term_d = mult_common * d[idx];
            if (sign == 1) sum_d += term_d; else sum_d -= term_d;

            /* contribution to d₂(m) (note the additional minus sign) */
            seq_t term_d2 = mult_common * d2[idx];
            if (sign == 1) sum_d2 -= term_d2; else sum_d2 += term_d2;

            /* prepare for next odd k */
            if (k + 2 <= m) {
                /* update binom to C(n_minus_1, r+2) */
                /* compute as: binom * (n_minus_1 - r) * (n_minus_1 - r - 1) / ((r+1)*(r+2)) */
                seq_t num = binom * (seq_t)(n_minus_1 - r) * (seq_t)(n_minus_1 - r - 1);
                seq_t den = (seq_t)(r + 1) * (seq_t)(r + 2);
                binom = num / den;
                r += 2;
            }

            sign = -sign;
            pow4 = pow4 * (seq_t)4;
        }

        d[m]  = sum_d;
        d2[m] = sum_d2;
    }

    comp = M;                              /* all values up to M computed */

    idx_t idx = 2 * n + 1;                  /* we need the (2n+1)-th coefficient */
    seq_t result = (d[idx] - d2[idx]) / (seq_t)2;
    return result;
}
