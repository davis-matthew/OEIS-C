/*
 * A208932  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208932; see the Formula section.
 * https://oeis.org/A208932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑1)-th coefficient in the row‑major, flattened
 *            triangle of coefficients of the polynomials v(n,x), where
 *            u(n,x)=u(n‑1,x)+2x·v(n‑1,x),
 *            v(n,x)=(x+1)·u(n‑1,x)+(x+1)·v(n‑1,x)+1, u(1,x)=1, v(1,x)=1
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper macros for allocation that work in C and C++ */
#ifdef __cplusplus
#  define SEQ_NEW(type, count) new type[count]
#  define SEQ_DELETE(arr, count) delete[] arr
#else
#  define SEQ_NEW(type, count) ((type*)malloc((count)*sizeof(type)))
#  define SEQ_DELETE(arr, count) free(arr)
#endif

/* Persistent state shared between calls */
static seq_t *coeffs = NULL;       /* flattened coefficients of all v(n,x) rows   */
static size_t coeffs_len = 0;      /* number of coefficients stored              */
static size_t coeffs_cap = 0;      /* current capacity of coeffs                 */

static idx_t  max_row = 0;         /* highest row that has been generated        */
static seq_t *u = NULL;            /* coefficients of u(n‑1,x)                    */
static seq_t *v = NULL;            /* coefficients of v(n‑1,x)                    */
static size_t u_len = 0, v_len = 0;

/* Ensure the coeffs array has enough capacity. */
static void ensure_coeffs(size_t needed)
{
    if (needed <= coeffs_cap) return;
    size_t new_cap = coeffs_cap ? coeffs_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(coeffs, new_cap * sizeof(seq_t));
    if (!tmp) exit(1); /* out of memory */
    coeffs = tmp;
    coeffs_cap = new_cap;
}

/* Compute all rows from max_row+1 up to target_row (inclusive). */
static void compute_up_to_row(idx_t target_row)
{
    if (target_row <= max_row) return;

    /* Initialise the first row on the first call. */
    if (max_row == 0) {
        u = SEQ_NEW(seq_t, 1);
        v = SEQ_NEW(seq_t, 1);
        u[0] = v[0] = (seq_t)1;
        u_len = v_len = 1;
        ensure_coeffs(1);
        coeffs[0] = (seq_t)1;
        coeffs_len = 1;
        max_row = 1;
    }

    for (idx_t r = max_row + 1; r <= target_row; ++r) {
        size_t len_prev = u_len;                /* length of previous row (r-1) */
        seq_t *new_u = SEQ_NEW(seq_t, r);       /* new row u(n,x) */
        seq_t *new_v = SEQ_NEW(seq_t, r);       /* new row v(n,x) */

        /* u[n] = u[n-1] + 2x·v[n-1] */
        new_u[0] = u[0];
        for (size_t k = 1; k < r; ++k) {
            seq_t u_prev = (k < len_prev) ? u[k] : (seq_t)0;
            seq_t v_prev = (k-1 < len_prev) ? v[k-1] : (seq_t)0;
            new_u[k] = u_prev + v_prev + v_prev; /* u_prev + 2*v_prev */
        }

        /* v[n] = (x+1)(u[n-1]+v[n-1]) + 1 */
        new_v[0] = u[0] + v[0] + (seq_t)1;
        for (size_t k = 1; k < r; ++k) {
            seq_t part1 = (k < len_prev) ? u[k] : (seq_t)0;
            seq_t part2 = (k < len_prev) ? v[k] : (seq_t)0;
            seq_t part3 = (k-1 < len_prev) ? u[k-1] : (seq_t)0;
            seq_t part4 = (k-1 < len_prev) ? v[k-1] : (seq_t)0;
            new_v[k] = part1 + part2 + part3 + part4;
        }

        /* Append the new v row to the global sequence. */
        ensure_coeffs(coeffs_len + r);
        for (size_t k = 0; k < r; ++k) {
            coeffs[coeffs_len++] = new_v[k];
        }

        /* Free old rows and adopt the new ones. */
        SEQ_DELETE(u, u_len);
        SEQ_DELETE(v, v_len);
        u = new_u; v = new_v;
        u_len = v_len = r;
    }

    max_row = target_row;
}

/* Return the minimal row r such that r*(r+1)/2 >= n. */
static idx_t row_from_index(idx_t n)
{
    idx_t low = 1, high = 1;
    while (((unsigned __int128)high * (high + 1)) / 2 < n) high <<= 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 val = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (val >= n) high = mid;
        else          low = mid + 1;
    }
    return low;
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* sequence starts at 1 */

    if (n > coeffs_len) {
        idx_t target_row = row_from_index(n);
        compute_up_to_row(target_row);
    }

    return coeffs[n - 1];
}
