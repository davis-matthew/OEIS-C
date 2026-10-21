/*
 * A208612  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208613; see the Formula section.
 * https://oeis.org/A208612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(n - t_{r-1} - 1) in the polynomial
 *            u(r,x), where t_m = m(m+1)/2 and the polynomials are generated
 *            by u(1,x)=1, v(1,x)=1 u(k,x)=u(k-1,x)+x*v(k-1,x)
 *            v(k,x)=(x+1)*u(k-1,x)+(x+1)*v(k-1,x)+1 for k>1
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the triangle of coefficients of u(k,x) up to
   row r = ceil((sqrt(8n+1)-1)/2), then returns the required element.
   The implementation uses different memory strategies for C and
   C++ to respect the requirements about seq_t possibly being an
   arbitrary‑precision class. */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is 1‑based.  Return 0 for n==0. */
    if (n == 0) return (seq_t)0;

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;

    /* Position j within row r (0‑based). */
    idx_t prev_tri = (r > 1 ? (r - 1) * r / 2 : 0);
    idx_t j = n - prev_tri - 1;

#if defined(__cplusplus)
    /* C++ path: use std::vector to ensure proper construction. */
    std::vector<seq_t> u_prev(1);
    std::vector<seq_t> v_prev(1);
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    for (idx_t cur = 2; cur <= r; ++cur) {
        std::vector<seq_t> u_cur(cur);
        std::vector<seq_t> v_cur(cur);

        /* u_cur[0] = u_prev[0] (no shift from v). */
        u_cur[0] = u_prev[0];

        /* Compute u_cur[p] = u_prev[p] + v_prev[p-1] (shifted). */
        for (idx_t p = 1; p < cur; ++p) {
            seq_t up = (p < u_prev.size() ? u_prev[p] : (seq_t)0);
            seq_t vp = v_prev[p - 1];
            u_cur[p] = up + vp;
        }

        /* Compute v_cur[p] = up + upm + vp + vpm + (p==0?1:0). */
        for (idx_t p = 0; p < cur; ++p) {
            seq_t up  = (p < u_prev.size() ? u_prev[p] : (seq_t)0);
            seq_t upm = (p > 0            ? u_prev[p - 1] : (seq_t)0);
            seq_t vp  = (p < v_prev.size() ? v_prev[p] : (seq_t)0);
            seq_t vpm = (p > 0            ? v_prev[p - 1] : (seq_t)0);
            seq_t val = up + upm + vp + vpm;
            if (p == 0) val += (seq_t)1;
            v_cur[p] = val;
        }

        /* Advance to next row. */
        u_prev.swap(u_cur);
        v_prev.swap(v_cur);
    }

    return u_prev[j];

#else
    /* C path: use malloc/calloc; works because seq_t is a simple integer type. */
    seq_t *u_prev = (seq_t *)calloc(1, sizeof(seq_t));
    seq_t *v_prev = (seq_t *)calloc(1, sizeof(seq_t));
    if (!u_prev || !v_prev) {
        free(u_prev); free(v_prev);
        return (seq_t)0;
    }
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    for (idx_t cur = 2; cur <= r; ++cur) {
        seq_t *u_cur = (seq_t *)calloc(cur, sizeof(seq_t));
        seq_t *v_cur = (seq_t *)calloc(cur, sizeof(seq_t));
        if (!u_cur || !v_cur) {
            free(u_prev); free(v_prev);
            free(u_cur); free(v_cur);
            return (seq_t)0;
        }

        /* u_cur[0] */
        u_cur[0] = u_prev[0];

        /* u_cur[p] */
        for (idx_t p = 1; p < cur; ++p) {
            seq_t up = (p < cur - 1 ? u_prev[p] : (seq_t)0);
            seq_t vp = v_prev[p - 1];
            u_cur[p] = up + vp;
        }

        /* v_cur[p] */
        for (idx_t p = 0; p < cur; ++p) {
            seq_t up  = (p < cur - 1 ? u_prev[p] : (seq_t)0);
            seq_t upm = (p > 0         ? u_prev[p - 1] : (seq_t)0);
            seq_t vp  = (p < cur - 1 ? v_prev[p] : (seq_t)0);
            seq_t vpm = (p > 0         ? v_prev[p - 1] : (seq_t)0);
            seq_t val = up + upm + vp + vpm;
            if (p == 0) val += (seq_t)1;
            v_cur[p] = val;
        }

        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
    }

    seq_t result = u_prev[j];
    free(u_prev);
    free(v_prev);
    return result;
#endif
}
