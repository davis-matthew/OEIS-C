/*
 * A208511  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208512; see the Formula section.
 * https://oeis.org/A208511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth coefficient when the rows of the polynomials
 * u(n,x) (defined below) are listed in order.  The polynomials satisfy
 *   u(1,x) = 1 ,   v(1,x) = 1
 *   u(n,x) = u(n‑1,x) + x·v(n‑1,x)
 *   v(n,x) = u(n‑1,x) + 2x·v(n‑1,x) + 1
 * For each n the row u(n,x) has n+1 coefficients; the sequence a(n)
 * is obtained by flattening all these rows: 1, 1,1, 1,3,2, 1,5,7,4, ...
 *
 * The function below generates the required coefficient without any
 * pre‑computed table.  It works for any size of seq_t (including an
 * arbitrary‑precision C++ class) and any idx_t (uint64_t).
 */

#include <stdlib.h>
/* For size_t in C++ (also defined in C) */
#if defined(__cplusplus)
    #include <cstddef>
#endif

seq_t a(idx_t n)
{
    /* Return 0 safely for n == 0 (out of the defined range). */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r for which 1 + 2 + ... + r >= n. */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Offset of the desired coefficient within row r (0‑based). */
    idx_t start  = (r - 1) * r / 2 + 1;
    idx_t offset = n - start;

    /* Allocate arrays for the previous row (row 1). */
#if defined(__cplusplus)
    seq_t *u_prev = new seq_t[1];
    seq_t *v_prev = new seq_t[1];
#else
    seq_t *u_prev = (seq_t*)malloc(sizeof(seq_t) * 1);
    seq_t *v_prev = (seq_t*)malloc(sizeof(seq_t) * 1);
#endif
    u_prev[0] = 1;
    v_prev[0] = 1;
    size_t prev_len = 1;

    /* Build rows iteratively up to row r. */
    for (idx_t k = 2; k <= r; ++k) {
        size_t cur_len = (size_t)k;
        seq_t *u_new = NULL;
        seq_t *v_new = NULL;
#if defined(__cplusplus)
        u_new = new seq_t[cur_len];
        v_new = new seq_t[cur_len];
#else
        u_new = (seq_t*)malloc(sizeof(seq_t) * cur_len);
        v_new = (seq_t*)malloc(sizeof(seq_t) * cur_len);
#endif

        /* Compute v(k,x). */
        v_new[0] = u_prev[0] + 1;              /* constant term */
        for (size_t i = 1; i < cur_len; ++i) {
            seq_t u_term = (i < prev_len) ? u_prev[i] : 0;
            seq_t shift  = v_prev[i - 1];
            shift = shift + shift;            /* multiply by 2 */
            v_new[i] = u_term + shift;
        }

        /* Compute u(k,x). */
        u_new[0] = u_prev[0];
        for (size_t i = 1; i < cur_len; ++i) {
            seq_t u_term = (i < prev_len) ? u_prev[i] : 0;
            seq_t v_term = v_prev[i - 1];
            u_new[i] = u_term + v_term;
        }

        /* Free the previous row. */
#if defined(__cplusplus)
        delete[] u_prev;
        delete[] v_prev;
#else
        free(u_prev);
        free(v_prev);
#endif
        u_prev  = u_new;
        v_prev  = v_new;
        prev_len = cur_len;
    }

    /* Extract the requested coefficient. */
    seq_t result = u_prev[(size_t)offset];

    /* Clean up memory. */
#if defined(__cplusplus)
    delete[] u_prev;
    delete[] v_prev;
#else
    free(u_prev);
    free(v_prev);
#endif

    return result;
}
