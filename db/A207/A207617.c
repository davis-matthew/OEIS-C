/*
 * A207617  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207616; see the Formula section.
 * https://oeis.org/A207617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n-1,x)+v(n-1,x)
 *            v(n,x)=x*u(n-1,x)+v(n-1,x)+1
 * The sequence a(n) consists of the coefficients of v(n,x) read
 * row‑by‑row, starting with a(1)=1.  This implementation
 * generates the required row and returns the requested coefficient.
 */

#include <stdlib.h>   /* malloc, free */
#include <stddef.h>   /* size_t */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n=0 */
        return (seq_t)0;

    if (n == 1)                 /* first term of row 1 */
        return (seq_t)1;

    /* Allocate two buffers each for u and v */
    /* maximum length needed for row r is ceil(r/2)+1  */
    /* we will never need more than row 2's length + 1 for safety */
    size_t max_len = (size_t)(n / 2) + 2;

#ifdef __cplusplus
    std::vector<seq_t> u_buf0(max_len), u_buf1(max_len);
    std::vector<seq_t> v_buf0(max_len), v_buf1(max_len);
    std::vector<seq_t> *u_cur  = &u_buf0, *u_next = &u_buf1;
    std::vector<seq_t> *v_cur  = &v_buf0, *v_next = &v_buf1;
#else
    seq_t *u_buf0 = (seq_t *)malloc(sizeof(seq_t) * max_len);
    seq_t *u_buf1 = (seq_t *)malloc(sizeof(seq_t) * max_len);
    seq_t *v_buf0 = (seq_t *)malloc(sizeof(seq_t) * max_len);
    seq_t *v_buf1 = (seq_t *)malloc(sizeof(seq_t) * max_len);
    if (!u_buf0 || !u_buf1 || !v_buf0 || !v_buf1) {
        free(u_buf0); free(u_buf1); free(v_buf0); free(v_buf1);
        return (seq_t)0;
    }
    seq_t *u_cur = u_buf0, *u_next = u_buf1;
    seq_t *v_cur = v_buf0, *v_next = v_buf1;
#endif

    size_t u_len = 1, v_len = 1;          /* lengths of current polynomials */

    /* initialise row 1: u = 1, v = 1 */
#ifdef __cplusplus
    (*u_cur)[0] = (seq_t)1;
    (*v_cur)[0] = (seq_t)1;
#else
    u_cur[0] = (seq_t)1;
    v_cur[0] = (seq_t)1;
#endif

    size_t cum = 1;                       /* number of terms seen so far (row 1) */
    idx_t row = 1;                        /* current row number */

    while (cum < n) {
        ++row;                            /* move to next row */

        /* lengths for the next row */
        idx_t u_next_len_idx = ((row - 1) / 2) + 1;
        idx_t v_next_len_idx = (row / 2) + 1;
        size_t u_next_len = (size_t)u_next_len_idx;
        size_t v_next_len = (size_t)v_next_len_idx;

        /* compute u_next = u_cur + v_cur (term‑wise) */
        for (size_t i = 0; i < u_next_len; ++i) {
            seq_t val = (seq_t)0;
#ifdef __cplusplus
            if (i < u_len) val += (*u_cur)[i];
            if (i < v_len) val += (*v_cur)[i];
            (*u_next)[i] = val;
#else
            if (i < u_len) val += u_cur[i];
            if (i < v_len) val += v_cur[i];
            u_next[i] = val;
#endif
        }

        /* compute v_next = v_cur + 1 + x*u_cur */
        for (size_t i = 0; i < v_next_len; ++i) {
            seq_t val = (seq_t)0;
#ifdef __cplusplus
            if (i < v_len) val += (*v_cur)[i];
            if (i == 0) val += (seq_t)1;
            if (i > 0 && i - 1 < u_len) val += (*u_cur)[i - 1];
            (*v_next)[i] = val;
#else
            if (i < v_len) val += v_cur[i];
            if (i == 0) val += (seq_t)1;
            if (i > 0 && i - 1 < u_len) val += u_cur[i - 1];
            v_next[i] = val;
#endif
        }

        /* update cumulative count */
        cum += v_next_len;
        if (cum >= n) {
            /* desired term lies in this row */
            idx_t offset = n - (cum - v_next_len_idx) - 1;   /* 0‑based */
            seq_t result;
#ifdef __cplusplus
            result = (*v_next)[(size_t)offset];
#else
            result = v_next[(size_t)offset];
#endif
            /* free memory */
#ifndef __cplusplus
            free(u_buf0); free(u_buf1); free(v_buf0); free(v_buf1);
#endif
            return result;
        }

        /* prepare for next iteration: swap buffers */
#ifdef __cplusplus
        std::swap(u_cur, u_next);
        std::swap(v_cur, v_next);
#else
        seq_t *tmp;
        tmp = u_cur;  u_cur = u_next;  u_next = tmp;
        tmp = v_cur;  v_cur = v_next;  v_next = tmp;
#endif

        u_len = u_next_len;
        v_len = v_next_len;
    }

    /* Should not reach here for n>=1 */
#ifndef __cplusplus
    free(u_buf0); free(u_buf1); free(v_buf0); free(v_buf1);
#endif
    return (seq_t)0;
}
