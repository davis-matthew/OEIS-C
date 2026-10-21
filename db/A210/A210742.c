/*
 * A210742  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210741; see the Formula section.
 * https://oeis.org/A210742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A210742): a(n) is the flattened
   sequence of coefficients of the polynomial v(n,x) defined by:
   u(1,x)=1, v(1,x)=1,
   u(n,x)=2x*u(n-1,x)+x*v(n-1,x)+1,
   v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1.
   a(1)=1. */

/* The sequence is obtained by:
   For each n≥1, compute v(n,x) and list its coefficients
   from constant term up to x^(n-1).  Concatenate these rows.
*/

#include <stdlib.h>    /* malloc, free, realloc */

static seq_t *flat  = NULL;   /* flattened coefficients */
static size_t  flat_len = 0;  /* number of terms stored */
static size_t  flat_cap = 0;  /* capacity of flat array */

static seq_t *u  = NULL;      /* coefficients of u(n,x) for current n */
static seq_t *v  = NULL;      /* coefficients of v(n,x) for current n */
static size_t cur_len = 0;    /* number of coefficients in current row */

/* Ensure flat array has at least `needed` capacity. */
static void
ensure_flat_capacity(size_t needed)
{
    if (needed > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 4;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
        flat = tmp;
        flat_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    idx_t target = n;         /* target 1‑based index */

    /* Initial row (n = 1) */
    if (cur_len == 0) {
        const size_t init_len = 1;
        u = (seq_t *)malloc(init_len * sizeof(seq_t));
        v = (seq_t *)malloc(init_len * sizeof(seq_t));
        u[0] = 1;
        v[0] = 1;

        ensure_flat_capacity(1);
        flat[0] = 1;
        flat_len = 1;
        cur_len = 1;
    }

    /* Generate rows until the flat array covers the requested index. */
    while (flat_len < (size_t)target) {
        size_t next_len = cur_len + 1;

        seq_t *u_next = (seq_t *)malloc(next_len * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(next_len * sizeof(seq_t));

        /* k = 0 */
        u_next[0] = 1;
        v_next[0] = u[0] + v[0] + (seq_t)1;

        /* k >= 1 */
        for (size_t k = 1; k < next_len; ++k) {
            seq_t uprev_k_1 = u[k-1];
            seq_t vprev_k_1 = v[k-1];
            /* u(next)[k] = 2*u(prev)[k-1] + v(prev)[k-1]  */
            u_next[k] = uprev_k_1 + uprev_k_1 + vprev_k_1;

            /* v(next)[k] = u(prev)[k] + u(prev)[k-1] + v(prev)[k] + v(prev)[k-1] */
            seq_t a = (k < cur_len) ? u[k] : (seq_t)0;
            seq_t b = uprev_k_1;
            seq_t c = (k < cur_len) ? v[k] : (seq_t)0;
            seq_t d = vprev_k_1;
            v_next[k] = a + b + c + d;
        }

        /* Append new coefficients to the flattened array. */
        ensure_flat_capacity(flat_len + next_len);
        for (size_t k = 0; k < next_len; ++k)
            flat[flat_len + k] = v_next[k];
        flat_len += next_len;

        /* Advance to the next row: free old arrays and store new ones. */
        free(u);
        free(v);
        u = u_next;
        v = v_next;
        cur_len = next_len;
    }

    return flat[(size_t)target - 1];
}
