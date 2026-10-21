/*
 * A227543  Triangle defined by g.f. A(x,q) such that: A(x,q) = 1 +
 *          x*A(q*x,q)*A(x,q), as read by terms k=0..n*(n-1)/2 in rows n>=0.
 * https://oeis.org/A227543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 393, u32 n <= 1835, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes A227543: the triangle defined by the generating
   function 
        A(x,q) = 1 + x*A(q*x,q)*A(x,q),
   read row‑by‑row, left to right.
   We build the triangle polynomially using the recurrence
        P_0 = 1,
        P_1 = 1,
        for r ≥ 2:
            P_r = P_{r-1} + q^{r-1}*P_{r-1}
                  + Σ_{m=1}^{r-2} q^m * P_m * P_{r-1-m},
   where P_r is the polynomial for coefficient of x^r.
   The flattened sequence a(n) is obtained by concatenating the rows.
   The implementation is fully generic in the integer type seq_t and idx_t
   (provided by the build) and works for 64‑bit indices and arbitrary‑precision
   values.  For indices up to ~10 000 the number of rows needed is < 40,
   so the routine stays well below the runtime limits. 
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t sum = 0;      /* cumulative count of terms up to previous row */
    idx_t r = 0;
    for (;;) {
        idx_t len = r * (r - 1) / 2 + 1;
        if (sum + len > n) break;
        sum += len;
        ++r;
    }
    idx_t offset = n - sum;          /* position within row r */

    /* allocate arrays for polynomials up to row r */
    seq_t **poly = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
    size_t *len_arr = (size_t*)malloc((r + 1) * sizeof(size_t));

    /* base rows */
    poly[0] = (seq_t*)malloc(sizeof(seq_t));
    poly[0][0] = (seq_t)1;
    len_arr[0] = 1;

    if (r >= 1) {
        poly[1] = (seq_t*)malloc(sizeof(seq_t));
        poly[1][0] = (seq_t)1;
        len_arr[1] = 1;
    }

    /* compute rows 2..r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        size_t cur_len = (size_t)cur * (cur - 1) / 2 + 1;
        seq_t *cur_arr = (seq_t*)calloc(cur_len, sizeof(seq_t));

        /* add P_{cur-1} */
        size_t len_prev = len_arr[cur - 1];
        for (size_t i = 0; i < len_prev; ++i) {
            cur_arr[i] += poly[cur - 1][i];
        }

        /* add q^{cur-1} * P_{cur-1}  (shift by cur-1) */
        for (size_t i = 0; i < len_prev; ++i) {
            cur_arr[(size_t)(cur - 1) + i] += poly[cur - 1][i];
        }

        /* add Σ_{m=1}^{cur-2} q^m * P_m * P_{cur-1-m} */
        for (idx_t m = 1; m <= cur - 2; ++m) {
            seq_t *Pm = poly[m];
            size_t lenPm = len_arr[m];
            seq_t *Pn = poly[cur - 1 - m];
            size_t lenPn = len_arr[cur - 1 - m];

            for (size_t i = 0; i < lenPm; ++i) {
                seq_t a = Pm[i];
                if (a == (seq_t)0) continue;
                for (size_t j = 0; j < lenPn; ++j) {
                    size_t idx = (size_t)m + i + j;
                    cur_arr[idx] += a * Pn[j];
                }
            }
        }

        poly[cur] = cur_arr;
        len_arr[cur] = cur_len;
    }

    /* result */
    seq_t result = poly[r][offset];

    /* free allocated memory */
    for (idx_t i = 0; i <= r; ++i) {
        free(poly[i]);
    }
    free(poly);
    free(len_arr);

    return result;
}
