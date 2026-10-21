/*
 * A209999  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210287; see the Formula section.
 * https://oeis.org/A209999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x)+1, *
 *            v(n,x)=u(n-1,x)+(x+1)*v(n-1,x)+1, * u(1,x)=v(1,x)=1. * a(n) is
 *            the nth coefficient in the triangle of the u(n,x) coefficients,
 *            * read row‑by‑row (row 1: 1; row 2: 2,2; row 3: 4,6,3; …)
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Platform­‑independent allocation for seq_t arrays.
 * Handles the case where seq_t is a C++ arbitrary‑precision class
 * (then constructors are needed) and the case where seq_t is a
 * fundamental integer type (C compiler). */
static seq_t *alloc_seq_array(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)malloc(len * sizeof(seq_t));
#endif
}
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

/* The sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n (triangle number). */
    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;   /* grow high */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;   /* row number (1‑based) */

    /* Allocate the first row (u=1, v=1). */
    size_t len_prev = 1;
    seq_t *u_prev = alloc_seq_array(len_prev);
    seq_t *v_prev = alloc_seq_array(len_prev);
    if (!u_prev || !v_prev) {
        free_seq_array(u_prev);
        free_seq_array(v_prev);
        return 0;
    }
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Build rows up to r. */
    for (idx_t row = 2; row <= r; ++row) {
        size_t len_cur = row;
        seq_t *u_cur = alloc_seq_array(len_cur);
        seq_t *v_cur = alloc_seq_array(len_cur);
        if (!u_cur || !v_cur) {
            free_seq_array(u_prev);
            free_seq_array(v_prev);
            free_seq_array(u_cur);
            free_seq_array(v_cur);
            return 0;
        }

        /* Compute u(row,x) coefficients. */
        for (size_t i = 0; i < len_cur; ++i) {
            seq_t term = 0;
            if (i > 0)
                term += u_prev[i - 1];
            if (i > 0)
                term += v_prev[i - 1];
            if (i < len_prev)
                term += v_prev[i];
            if (i == 0)
                term += (seq_t)1;
            u_cur[i] = term;
        }

        /* Compute v(row,x) coefficients. */
        for (size_t i = 0; i < len_cur; ++i) {
            seq_t term = 0;
            if (i < len_prev)
                term += u_prev[i];
            if (i > 0)
                term += v_prev[i - 1];
            if (i < len_prev)
                term += v_prev[i];
            if (i == 0)
                term += (seq_t)1;
            v_cur[i] = term;
        }

        /* Free previous row arrays. */
        free_seq_array(u_prev);
        free_seq_array(v_prev);

        /* Move to next row. */
        u_prev = u_cur;
        v_prev = v_cur;
        len_prev = len_cur;
    }

    /* Index of target coefficient within row r. */
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t pos_in_row = (idx_t)(n - before - 1);

    seq_t result = u_prev[pos_in_row];

    free_seq_array(u_prev);
    free_seq_array(v_prev);
    return result;
}
