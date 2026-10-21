/*
 * A210802  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210801; see the Formula section.
 * https://oeis.org/A210802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Polynomials u(n,x) and v(n,x) are defined by
 *    u(n,x) = u(n-1,x) + (x+1)·v(n-1,x) + 1
 *    v(n,x) = (x+2)·u(n-1,x) + (x-1)·v(n-1,x) + 1
 * with u(1,x)=1, v(1,x)=1.
 * The flattened sequence a(n) is the coefficients of v(n,x) arranged
 * row by row.  Row n has n coefficients and ends with F(n+1) (the
 * (n+1)st Fibonacci number). */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* Determine which row contains the nth term. */
    idx_t r = 1;
    idx_t sum = 1; /* total terms up to row r */
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* index of the coefficient within row r (0‑based) */
    idx_t pos = n - (sum - r) - 1;

    /* Allocate storage for the current row of u and v. */
    seq_t* u = NULL;
    seq_t* v = NULL;
    idx_t cur_len = 1;

    /* Initial row (n=1): u=1, v=1 */
    u = (seq_t*)malloc(sizeof(seq_t));
    v = (seq_t*)malloc(sizeof(seq_t));
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    if (r == 1) {                     /* answer is the single coefficient */
        seq_t result = v[pos];
        free(u);
        free(v);
        return result;
    }

    /* Generate rows 2 … r */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t prev_len = cur_len;     /* length of previous row */
        idx_t next_len = row;         /* length of current row */
        seq_t* next_u = (seq_t*)malloc(next_len * sizeof(seq_t));
        seq_t* next_v = (seq_t*)malloc(next_len * sizeof(seq_t));

        /* i = 0 */
        next_u[0] = u[0] + v[0] + (seq_t)1;           /* u_next[0] */
        seq_t two_u0 = u[0];
        two_u0 += u[0];                                /* 2 * u[0] */
        next_v[0] = two_u0 + (seq_t)1 - v[0];           /* v_next[0] */

        /* i >= 1 */
        for (idx_t i = 1; i < next_len; ++i) {
            seq_t pu  = (i < prev_len) ? u[i]  : (seq_t)0;
            seq_t pv  = (i < prev_len) ? v[i]  : (seq_t)0;
            seq_t pu_prev = u[i-1];
            seq_t pv_prev = v[i-1];

            /* u_next[i] = pu + pv + pv_prev */
            next_u[i] = pu;
            next_u[i] += pv;
            next_u[i] += pv_prev;

            /* v_next[i] = 2*pu + pu_prev + pv_prev - pv */
            seq_t tmp = pu;
            tmp += pu;                 /* 2*pu */
            tmp += pu_prev;
            tmp += pv_prev;
            tmp -= pv;
            next_v[i] = tmp;
        }

        /* If this is the target row, return the required coefficient */
        if (row == r) {
            seq_t result = next_v[pos];
            free(u);
            free(v);
            free(next_u);
            free(next_v);
            return result;
        }

        /* Prepare for next iteration */
        free(u);
        free(v);
        u = next_u;
        v = next_v;
        cur_len = next_len;
    }

    /* Should never reach here */
    return (seq_t)0;
}
