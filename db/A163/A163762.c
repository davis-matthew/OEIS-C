/*
 * A163762  Triangle of coefficients of polynomials
 *          H(n,x)=(U^n+L^n)/2+(U^n-L^n)/(2d), where U=x+d, L=x-d,
 *          d=(x+4)^(1/2).
 * https://oeis.org/A163762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   H(n,x)=2*x*H(n-1,x)-(x^2-x-4)*H(n-2,x), H(0,x)=1, H(1,x)=x+1.
 *            The sequence a(n) is the flattened triangle of coefficients of
 *            H(n-1,x) written from highest degree to constant term
 * Range:     u8 n <= 25, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine the row r and the position in that row */
    idx_t r = 1;
    while ((idx_t)r * ((idx_t)r + 1) / 2 < n) ++r;
    idx_t prev_tri = (idx_t)(r - 1) * (idx_t)r / 2;            /* T(r-1) */
    idx_t pos_desc = n - prev_tri - 1;                         /* 0‑based, high→low */

    /* cache of previously computed rows  (row 1 corresponds to H_0) */
    static seq_t **rows = NULL;
    static size_t cached_r = 0;    /* number of rows already computed */

    /* compute missing rows up to r */
    while (cached_r < (size_t)r) {
        size_t len = cached_r + 1;            /* length of the new row */
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!row) abort();                    /* memory failure */

        if (cached_r == 0) {                  /* H_0 */
            row[0] = 1;
        } else if (cached_r == 1) {           /* H_1 */
            row[0] = 1;
            row[1] = 1;
        } else {
            /* r >= 3, build using the recurrence with only additions */
            seq_t *prev1 = rows[cached_r - 1];   /* H_(r-2) */
            seq_t *prev2 = rows[cached_r - 2];   /* H_(r-3) */

            /* temporary arrays for positive and negative contributions */
            seq_t *pos = (seq_t *)calloc(len, sizeof(seq_t));
            seq_t *neg = (seq_t *)calloc(len, sizeof(seq_t));
            if (!pos || !neg) abort();

            /* 2*x*prev1  (shift by 1, multiply by 2) */
            for (idx_t i = 0; i < len - 1; ++i) {
                pos[i + 1] += prev1[i] * (seq_t)2;
            }

            /* +x*prev2  (shift by 1)  */
            /* +4*prev2  (no shift)   */
            /* -x^2*prev2 (shift by 2) */
            for (idx_t i = 0; i < len - 2; ++i) {
                pos[i + 1] += prev2[i];               /* +x*prev2 */
                pos[i]     += prev2[i] * (seq_t)4;    /* +4*prev2 */
                neg[i + 2] += prev2[i];               /* -x^2*prev2 */
            }

            /* combine to obtain the final coefficients */
            for (idx_t i = 0; i < len; ++i) {
                row[i] = pos[i] - neg[i];
            }

            free(pos);
            free(neg);
        }

        /* enlarge the cache array and store the new row */
        if (rows == NULL) {
            rows = (seq_t **)malloc(sizeof(seq_t *));
        } else {
            rows = (seq_t **)realloc(rows, sizeof(seq_t *) * (cached_r + 1));
        }
        if (!rows) abort();
        rows[cached_r] = row;
        ++cached_r;
    }

    /* fetch the desired coefficient */
    /* ascending index = (row degree) - pos_desc = (r-1) - pos_desc */
    size_t asc_index = (size_t)(r - 1 - pos_desc);
    return rows[r - 1][asc_index];
}
