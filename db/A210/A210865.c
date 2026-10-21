/*
 * A210865  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210864; see the Formula section.
 * https://oeis.org/A210865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened sequence of coefficients of the
 *            polynomials v(n,x) defined by: u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n-1,x)+x*v(n-1,x)+1 v(n,x)=(x+n)*u(n-1,x)+x*v(n-1,x)
 *            The k‑th element of row n (k=0…n‑1) is v(n,k), which is listed
 *            in a(n) in reading order by rows
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
    */

#include <stdlib.h>
#include <string.h>

/* static state */
static seq_t *u_prev = NULL;     /* coefficients of u(n-1,x) */
static seq_t *v_prev = NULL;     /* coefficients of v(n-1,x) */
static idx_t  prev_len = 0;      /* length of previous row (n-1) */

/* cache of a(n) values, 0‑based index: a_cache[0] = a(1) */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;   /* number of terms stored */
static size_t a_cache_cap = 0;

/* append a value to cache, grow if necessary */
static void append_to_cache(seq_t val)
{
    if (a_cache_len == a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) { /* allocation failed */
            exit(EXIT_FAILURE);
        }
        if (a_cache) {
            memcpy(new_arr, a_cache, a_cache_len * sizeof(seq_t));
            free(a_cache);
        }
        a_cache = new_arr;
        a_cache_cap = new_cap;
    }
    a_cache[a_cache_len++] = val;
}

/* compute and retain all rows up to row n (inclusive) */
static void compute_up_to(idx_t n)
{
    /* n is 1‑based */
    if (a_cache_len >= (size_t)n) return;          /* already computed */

    /* if starting fresh, initialize row 1 */
    if (prev_len == 0) {
        prev_len = 1;
        u_prev = (seq_t *)malloc(sizeof(seq_t));
        v_prev = (seq_t *)malloc(sizeof(seq_t));
        u_prev[0] = 1;
        v_prev[0] = 1;
        append_to_cache(v_prev[0]);                 /* a(1)=1 */
    }

    /* iterate rows from prev_len+1 up to n (inclusive) */
    for (idx_t r = prev_len + 1; r <= n; ++r) {
        idx_t new_len = r;
        seq_t *u_new = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *v_new = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!u_new || !v_new) exit(EXIT_FAILURE);

        for (idx_t k = 0; k < new_len; ++k) {
            seq_t u_prev_k   = (k < prev_len) ? u_prev[k] : 0;
            seq_t v_prev_k_1 = (k > 0) ? v_prev[k - 1] : 0;
            seq_t u_prev_k_1 = (k > 0) ? u_prev[k - 1] : 0;

            /* u(n,x) = u(n-1,x) + x*v(n-1,x) + 1 */
            u_new[k] = u_prev_k + v_prev_k_1 + (k == 0 ? (seq_t)1 : 0);

            /* v(n,x) = (x+n)*u(n-1,x) + x*v(n-1,x) */
            v_new[k] = (seq_t)r * u_prev_k + u_prev_k_1 + v_prev_k_1;
        }

        /* append the row's coefficients to the global cache */
        for (idx_t k = 0; k < new_len; ++k) {
            append_to_cache(v_new[k]);
        }

        /* free old arrays and advance pointers */
        free(u_prev);
        free(v_prev);
        u_prev = u_new;
        v_prev = v_new;
        prev_len = new_len;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* n is 1‑based; undefined for n<1 */
    if (n < 1) return (seq_t)0; /* or handle as you wish */

    compute_up_to(n);
    return a_cache[n - 1];
}
