/*
 * A173585  Triangle T(n, k, q) = c(n, q)/(c(k, q)*c(n-k, q)), where c(n, q) =
 *          Product_{j=1..n} t(2*j, q), t(n, q) = (1/4)*( (2 + sqrt(q))^n + (2
 *          - sqrt(q))^n - 2 ), and q = 3, read by rows.
 * https://oeis.org/A173585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k, 3), * where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, * and T(r, k, q) = c(r, q)/(c(k, q)*c(r-k, q)), *
 *            c(n, q) = product_{j=1..n} t(2*j, q), * t(m, q) =
 *            (1/4)*((2+sqrt(q))^m + (2-sqrt(q))^m - 2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 47, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes the sequence t(2*j,3) and
 * uses the multiplicative recurrence
 *    T(r,0)=1,   T(r,k+1)=T(r,k)*t(r-k)/t(k+1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- cached pre‑computation of t(2*j,3) and auxiliary values ----- */
static seq_t *t_arr   = NULL;   /* t_arr[j] = t(2*j,3)   (1‑based) */
static seq_t *a_arr   = NULL;   /* a_arr[m] = (2+√3)^m + (2-√3)^m  (0‑based) */
static idx_t  t_size  = 0;      /* largest j currently available */
static idx_t  a_len   = 0;      /* current length of a_arr */

/* extend cache up to new_row (inclusive) if needed */
static void ensure_precomp(idx_t new_row)
{
    if (new_row <= t_size)
        return;                      /* already available */

    idx_t old_t_size = t_size;
    t_size = new_row;

    /* allocate t_arr (1‑based) */
    size_t new_t_bytes = (size_t)(t_size + 1) * sizeof(seq_t);
    t_arr = (seq_t *)realloc(t_arr, new_t_bytes);

    /* allocate a_arr: length = 2*t_size + 1 (allow even indices up to 2*t_size) */
    idx_t new_a_len = (idx_t)(2 * t_size + 1);
    size_t new_a_bytes = (size_t)new_a_len * sizeof(seq_t);
    a_arr = (seq_t *)realloc(a_arr, new_a_bytes);

    /* initialise auxiliary recurrence only when we need at least one t value */
    if (old_t_size == 0 && t_size > 0) {
        a_arr[0] = (seq_t)2;         /* a_0 = 2                        */
        a_arr[1] = (seq_t)4;         /* a_1 = 4                        */
        a_len    = 2;                /* we have indices 0 .. 1          */
    }

    /* extend a_arr using the linear recurrence a_m = 4*a_{m-1} - a_{m-2} */
    idx_t start = a_len;
    for (idx_t i = start; i < (idx_t)new_a_len; ++i) {
        a_arr[i] = (seq_t)4 * a_arr[i - 1] - a_arr[i - 2];
    }
    a_len = new_a_len;

    /* compute t_arr for new j values */
    for (idx_t j = old_t_size + 1; j <= t_size; ++j) {
        /* even index 2*j in a_arr */
        idx_t even_idx = 2 * j;
        t_arr[j] = (a_arr[even_idx] - (seq_t)2) / (seq_t)4;
    }
}

/* ----- convert linear index to triangle coordinates ----- */
static void antidiagonal_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    *row = r;
    *col = n - r * (r + 1) / 2;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);

    /* ensure that all necessary t values are available */
    ensure_precomp(row);

    /* compute T(row, col, 3) using the product recurrence */
    seq_t val = 1;   /* works for both integer and big‑int types */
    for (idx_t i = 0; i < col; ++i) {
        val = val * t_arr[row - i];
        val = val / t_arr[i + 1];   /* division is exact for the sequence */
    }
    return val;
}
