/*
 * A207610  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207611; see the Formula section.
 * https://oeis.org/A207610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑th) coefficient read row‑wise from the triangle
 *            whose rows are coefficients of the polynomials u(n,x) defined by
 *            u(1,x) = 1, v(1,x) = 1, u(n,x) = u(n-1,x) + v(n-1,x), v(n,x) =
 *            u(n-1,x) + x*v(n-1,x) + 1 (n > 1), where the k‑th entry of row n
 *            (0‑based degree) is the coefficient of x^...
 * Range:     u8 n <= 47, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n <= 1) return (seq_t)1;

    /* Determine the row r that contains the n-th coefficient.
       Row lengths are 1,1,2,3,4,...  The cumulative counts are
       S(r)=1 + (r-1)r/2 for r >= 1.  Find the smallest r with S(r) >= n. */
    idx_t r = 1;
    idx_t cumulative = 1;                    /* S(1) */
    while (cumulative < n)
    {
        ++r;
        cumulative = 1 + (r - 1) * r / 2;    /* compute S(r) */
    }

    /* Offset k (0‑based) within row r */
    idx_t cumulative_prev = (r > 1)
                            ? (1 + (r - 2) * (r - 1) / 2)
                            : 0;
    idx_t k = n - cumulative_prev - 1;       /* 0‑based index in row r */

    /* Build rows until r, keeping only the previous u and v arrays. */
    /* Row 1 */
    seq_t *u_prev = NULL;
    seq_t *v_prev = NULL;
    idx_t prev_len = 1;      /* length of u_prev */
    idx_t prev_v_len = 1;    /* length of v_prev */

    u_prev = (seq_t *)malloc(sizeof(seq_t));
    *u_prev = (seq_t)1;
    v_prev = (seq_t *)malloc(sizeof(seq_t));
    *v_prev = (seq_t)1;

    /* Generate rows 2 .. r */
    for (idx_t cur = 2; cur <= r; ++cur)
    {
        /* New lengths:
           length of u_next = length of previous v (prev_v_len),
           length of v_next = prev_v_len + 1.  */
        idx_t len_u = prev_v_len;
        idx_t len_v = prev_v_len + 1;

        seq_t *u_next = (seq_t *)malloc(len_u * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(len_v * sizeof(seq_t));

        /* Compute u_next = u_prev + v_prev (degree‑wise) */
        for (idx_t i = 0; i < len_u; ++i)
        {
            seq_t a = (i < prev_len) ? u_prev[i] : (seq_t)0;
            seq_t b = (i < prev_v_len) ? v_prev[i] : (seq_t)0;
            u_next[i] = a + b;
        }

        /* Compute v_next = u_prev + x*v_prev + 1 (degree‑wise) */
        for (idx_t i = 0; i < len_v; ++i)
        {
            seq_t a = (i < prev_len) ? u_prev[i] : (seq_t)0;
            seq_t b = (i > 0) ? v_prev[i - 1] : (seq_t)0;
            seq_t c = (i == 0) ? (seq_t)1 : (seq_t)0;
            v_next[i] = a + b + c;
        }

        /* Free old arrays and update pointers */
        free(u_prev);
        free(v_prev);
        u_prev = u_next;
        v_prev = v_next;
        prev_len = len_u;
        prev_v_len = len_v;
    }

    /* u_prev now holds the coefficients of row r (length r-1 for r>1). */
    seq_t result = u_prev[k];

    /* Clean up */
    free(u_prev);
    free(v_prev);
    return result;
}
