/*
 * A209704  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209703; see the Formula section.
 * https://oeis.org/A209704
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
 * Formula: a(n) is the nth entry of the triangle of coefficients
 * of the polynomials v(n,x) which satisfy 
 *   u(1,x)=1,  v(1,x)=1
 *   u(n,x)=x*u(n-1,x)+x*v(n-1,x)
 *   v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1
 * The n‑th term is the coefficient of x^(col-1) in v(row,x),
 * where row is the unique integer with row*(row+1)/2 ≥ n.
 */
#include <stdlib.h>
#include <stdint.h>

/* Determine floor(sqrt(x)) for 128‑bit inputs. */
static inline idx_t isqrt128(unsigned __int128 x)
{
    idx_t low = 0, high = (idx_t)-1, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
            res = mid;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* Find the row index r such that r*(r+1)/2 ≥ n. */
static inline idx_t find_row(idx_t n)
{
    if (n <= 1) return 1;
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt128(val);            /* floor sqrt(8n+1) */
    idx_t r = (s - 1) / 2;              /* floor part */
    if ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
    return r;
}

/* Compute the nth term of A209704. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out‑of‑range, undefined behaviour */

    /* Determine row r and column col of the desired term. */
    idx_t r   = find_row(n);
    idx_t prev_total = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t col = n - prev_total;   /* 1‑based column within the row */

    /* Allocate temporary arrays. */
    size_t max_len = (size_t)r;   /* maximum length needed */
    seq_t *u_cur  = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *v_cur  = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *u_next = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *v_next = (seq_t *)malloc(max_len * sizeof(seq_t));
    if (!u_cur || !v_cur || !u_next || !v_next) {
        /* Memory allocation failed. Return 0 as a safe fallback. */
        free(u_cur); free(v_cur); free(u_next); free(v_next);
        return (seq_t)0;
    }

    /* Initial row (n = 1). */
    u_cur[0] = (seq_t)1;
    v_cur[0] = (seq_t)1;

    if (r == 1) {
        seq_t result = v_cur[0];
        free(u_cur); free(v_cur); free(u_next); free(v_next);
        return result;
    }

    /* Generate rows up to r. */
    for (size_t row = 2; row <= r; ++row) {
        size_t curr_len = row;          /* number of coefficients in this row */

        /* v_next[0] uses values from the previous row. */
        v_next[0] = u_cur[0] + v_cur[0] + (seq_t)1;

        /* u_next[0] is always 0 for n > 1. */
        u_next[0] = (seq_t)0;

        /* Compute u_next coefficients for k = 1 .. curr_len-1. */
        for (size_t k = 1; k < curr_len; ++k) {
            u_next[k] = u_cur[k - 1] + v_cur[k - 1];
        }

        /* Compute v_next coefficients for k = 1 .. curr_len-1. */
        for (size_t k = 1; k < curr_len; ++k) {
            seq_t val = u_cur[k - 1];
            if (k < row - 1) {                 /* u_cur[k] and v_cur[k] exist */
                val += u_cur[k] + v_cur[k];
            }
            v_next[k] = val;
        }

        /* Swap current and next arrays for the next iteration. */
        seq_t *tmp;
        tmp = u_cur;  u_cur = u_next;  u_next = tmp;
        tmp = v_cur;  v_cur = v_next;  v_next = tmp;
    }

    /* Retrieve the desired coefficient. */
    seq_t result = v_cur[col - 1];

    /* Clean up. */
    free(u_cur);
    free(v_cur);
    free(u_next);
    free(v_next);

    return result;
}
