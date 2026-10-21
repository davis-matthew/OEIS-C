/*
 * A208338  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208339; see the Formula section.
 * https://oeis.org/A208338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208338 – triangle of coefficients of the polynomials
 * u(n,x) generated together with v(n,x) via
 *
 *   u(1,x)=1 ,  v(1,x)=1
 *   u(n,x)=u(n-1,x)+x*v(n-1,x)
 *   v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)
 *
 * The sequence is read row‑by‑row from the coefficients of u(n,x);
 * a(n) is the n‑th coefficient encountered in that reading order.
 * (No simple closed formula is known; the implementation follows
 * the above recurrence.)
 */

#include <stdlib.h>

static idx_t  max_row     = 0;   /* last completed row number   */
static idx_t  term_count  = 0;   /* number of sequence terms     */
static seq_t *results     = NULL;/* array holding a(1)..a(term_count) */
static size_t res_cap     = 0;   /* current capacity of results  */

static seq_t *u_prev = NULL;     /* coefficients of u(n-1,x)      */
static seq_t *v_prev = NULL;     /* coefficients of v(n-1,x)      */

/* ensure that at least n terms of the sequence are computed */
static void
ensure_terms(idx_t n)
{
    if (term_count >= n) return;   /* already have enough terms */

    /* first call – initialise row 1 */
    if (term_count == 0) {
        max_row = 1;
        u_prev = (seq_t*)malloc(sizeof(seq_t));
        v_prev = (seq_t*)malloc(sizeof(seq_t));
        if (!u_prev || !v_prev) exit(EXIT_FAILURE);
        u_prev[0] = 1;
        v_prev[0] = 1;

        res_cap = 1;
        results = (seq_t*)malloc(res_cap * sizeof(seq_t));
        if (!results) exit(EXIT_FAILURE);
        results[0] = 1;
        term_count = 1;
    }

    /* generate rows until we have n terms */
    while (term_count < n) {
        idx_t    new_row   = max_row + 1;        /* row number we are creating */
        size_t   new_len   = (size_t)new_row;    /* length of the new row       */

        seq_t *u_cur = (seq_t*)malloc(new_len * sizeof(seq_t));
        seq_t *v_cur = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!u_cur || !v_cur) exit(EXIT_FAILURE);

        for (idx_t k = 0; k < new_len; ++k) {
            seq_t part1 = (k < max_row) ? u_prev[k] : 0;
            seq_t part2 = (k > 0) ? v_prev[k-1] : 0;
            u_cur[k] = part1 + part2;

            seq_t val = 0;
            if (k < max_row)             val += u_prev[k];
            if (k > 0) {
                val += u_prev[k-1];
                val += (seq_t)2 * v_prev[k-1];
            }
            v_cur[k] = val;
        }

        /* expand results array if needed */
        size_t needed = term_count + new_len;
        if (needed > res_cap) {
            size_t new_cap = res_cap ? res_cap * 2 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            results = tmp;
            res_cap = new_cap;
        }

        /* append the new coefficients to the sequence */
        for (idx_t k = 0; k < new_len; ++k) {
            results[term_count + k] = u_cur[k];
        }
        term_count += new_len;

        /* free old row, prepare for next iteration */
        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
        max_row = new_row;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<=0; return 0 as safe default */
    ensure_terms(n);
    return results[n-1];
}
