/*
 * A140946  Triangle T(n,k) = (-2)^n*(-1)^k if k<n; T(n,n) =
 *          (-1)^(n+1)*A001045(n+1).
 * https://oeis.org/A140946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k) listed row‑wise.
 *            For a given index n determine the row r and column c by: c = n -
 *            r(r+1)/2 where r = floor((sqrt(8n+1)-1)/2) If c < r : a(n) =
 *            (-2)^r * (-1)^c If c == r : a(n) = (-1)^r * b(r+1) with the
 *            auxiliary sequence b(m) defined by b(1) = 1, b(2)...
 * Range:     i8 n <= 28, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t pow_minus2(idx_t exp) {
    seq_t result = 1;
    seq_t base = -2;
    idx_t e = exp;
    while (e) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1U;
    }
    return result;
}

/* Cache for the auxiliary sequence b(m) = A001045(m) */
static seq_t *b_cache = NULL;   /* array indexed by m, b_cache[0] unused */
static size_t b_len   = 0;       /* current length of b_cache (>= needed + 1) */

/* Ensure that b_cache has values for m = 1..max_m */
static void ensure_b_cache(idx_t max_m) {
    if (max_m < 1) return;
    if (b_len > max_m) return;
    size_t new_len = b_len ? b_len : 16;
    while (new_len <= (size_t)max_m) new_len <<= 1;
    seq_t *new_arr = (seq_t *)realloc(b_cache, new_len * sizeof(seq_t));
    if (!new_arr) exit(1);
    b_cache = new_arr;
    /* Fill missing values */
    if (b_len == 0) {
        b_cache[0] = 0;            /* unused */
        b_cache[1] = 1;            /* b(1) */
        if (new_len > 2) b_cache[2] = 1; /* b(2) */
    } else {
        /* b_cache[1] and b_cache[2] already set */
    }
    for (idx_t i = b_len; i <= (idx_t)max_m; ++i) {
        if (i == 1 || i == 2) {
            b_cache[i] = 1;        /* base cases already set */
        } else {
            b_cache[i] = b_cache[i-1] + (seq_t)2 * b_cache[i-2];
        }
    }
    b_len = (size_t)max_m + 1;
}

static seq_t get_b(idx_t m) {
    if (m < 1) return 0;
    ensure_b_cache(m);
    return b_cache[m];
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Determine row r and column c */
    idx_t row = 0;
    idx_t cum = 0; /* cumulative count of elements before current row */
    while (cum + row + 1 <= n) {
        cum += row + 1;
        row++;
    }
    idx_t col = n - cum; /* 0‑based column within the row */

    if (col < row) {
        seq_t val = pow_minus2(row);
        if (col & 1U) val = -val;
        return val;
    } else { /* col == row */
        seq_t val = get_b(row + 1);
        if (row & 1U) val = -val;
        return val;
    }
}
