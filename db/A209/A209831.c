/*
 * A209831  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209830; see the Formula section.
 * https://oeis.org/A209831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209831:  Triangle of coefficients of v(n,x), flattened
   (rows 1,2,3,…).  The coefficients are produced by the
   bi‑recurrence                                 
        u(1,x)=1, v(1,x)=1
        u(n,x)=x*u(n−1,x)+(x+1)*v(n−1,x)
        v(n,x)=(x+1)*u(n−1,x)+2x*v(n−1,x)
   The sequence a(n) lists, for n = 1,2,3,…, the coefficients of v(n,x)
   from the constant term up to the highest power.  Initial values:
   a(1)=1, a(2)=1, a(3)=3, a(4)=1, a(5)=5, a(6)=8, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global static state for memoization */
static seq_t *seq_cache = NULL;   /* flattened coefficients of v(x) */
static size_t seq_len = 0;        /* number of terms currently stored */
static size_t seq_cap = 0;        /* allocated capacity of seq_cache   */

static seq_t *u_prev = NULL;      /* coefficients of u(n‑1,x) */
static seq_t *v_prev = NULL;      /* coefficients of v(n‑1,x) */
static size_t prev_len = 0;       /* length of u_prev / v_prev      */

static size_t current_row = 0;    /* number of rows already generated */

/* Ensure seq_cache can hold at least 'need' elements.  */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate the next row of the triangle.
   The new row index is current_row + 1. */
static void generate_next_row(void)
{
    size_t new_row = current_row + 1;   /* 1‑based index of new row */
    size_t len = new_row;              /* number of coefficients in this row */

    seq_t *u_next = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *v_next = (seq_t *)malloc(len * sizeof(seq_t));

    if (new_row == 1) {
        /* Base case: u(1)=v(1)=1 */
        u_next[0] = 1;
        v_next[0] = 1;
    } else {
        for (size_t k = 0; k < len; ++k) {
            /* u(k) = x*u_prev + (x+1)*v_prev     */
            seq_t u_val = 0;
            if (k > 0)                            u_val += u_prev[k-1];
            if (k < prev_len)                     u_val += v_prev[k];
            if (k > 0 && (k-1) < prev_len)        u_val += v_prev[k-1];
            u_next[k] = u_val;

            /* v(k) = (x+1)*u_prev + 2x*v_prev   */
            seq_t v_val = 0;
            if (k > 0)                            v_val += u_prev[k-1];
            if (k < prev_len)                     v_val += u_prev[k];
            if (k > 0)                            v_val += 2 * v_prev[k-1];
            v_next[k] = v_val;
        }
    }

    /* Append the new row's coefficients of v to the flattened list */
    ensure_seq_cap(seq_len + len);
    for (size_t i = 0; i < len; ++i) {
        seq_cache[seq_len++] = v_next[i];
    }

    /* Free the previous row data */
    free(u_prev);
    free(v_prev);

    /* Store the new row for the next iteration */
    u_prev = u_next;
    v_prev = v_next;
    prev_len = len;
    current_row = new_row;
}

/* Ensure that the cache contains at least the first n terms.  */
static void grow_to(idx_t n)
{
    while ((idx_t)seq_len < n) {
        generate_next_row();
    }
}

/* Return the nth term of the flattened triangle (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined, but guard */
    grow_to(n);
    return seq_cache[(size_t)(n-1)];
}
