/*
 * A209771  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209772; see the Formula section.
 * https://oeis.org/A209771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(k) in polynomial u(r,x) * where r=
 *            smallest integer with r*(r+1)/2 >= n * and k = n - r*(r-1)/2 -
 *            1. * The coefficients of u and v satisfy * u(1,0)=v(1,0)=1 *
 *            u(n,k)=u(n‑1,k‑1)+v(n‑1,k‑1)+v(n‑1,k) *
 *            v(n,k)=u(n‑1,k‑1)+v(n‑1,k‑1)+u(n‑1,k)+ (k==0?1:0)
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n)
    {
        ++r;
        sum += r;          /* sum now = r*(r+1)/2 */
    }

    /* index of coefficient within the row (0‑based) */
    idx_t prev_sum = (r - 1) * r / 2;   /* (r-1)*r/2 = sum of first r-1 rows */
    idx_t k = n - prev_sum - 1;         /* 0 <= k < r */

    if (r == 1)                /* only constant term 1 */
        return 1;

    /* allocate arrays for previous row (length r-1) */
    seq_t *u_prev = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *v_prev = (seq_t *)calloc(r, sizeof(seq_t));
    u_prev[0] = 1;
    v_prev[0] = 1;

    /* compute subsequent rows up to row r */
    for (idx_t len = 2; len <= r; ++len)
    {
        seq_t *u_cur = (seq_t *)calloc(len, sizeof(seq_t));
        seq_t *v_cur = (seq_t *)calloc(len, sizeof(seq_t));

        for (idx_t j = 0; j < len; ++j)
        {
            seq_t u_val = 0;
            seq_t v_val = 0;

            if (j > 0)                /* terms involving k-1 */
            {
                u_val += u_prev[j - 1];
                u_val += v_prev[j - 1];

                v_val += u_prev[j - 1];
                v_val += v_prev[j - 1];
            }

            if (j < len - 1)          /* terms involving k (same row) */
            {
                u_val += v_prev[j];
                v_val += u_prev[j];
            }

            if (j == 0)                /* constant term of v has +1 */
                v_val += 1;

            u_cur[j] = u_val;
            v_cur[j] = v_val;
        }

        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
    }

    seq_t result = u_prev[k];          /* desired coefficient */

    free(u_prev);
    free(v_prev);
    return result;
}
