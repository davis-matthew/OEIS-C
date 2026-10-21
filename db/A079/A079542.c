/*
 * A079542  Triangular array: T(n,1) = T(n,n) = n and T(n,k) = lcm(T(n-1,k-1),
 *          T(n-1,k)) for 1 < k < n.
 * https://oeis.org/A079542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n,1) = T(n,n) = n; 
   T(n,k) = lcm(T(n-1,k-1), T(n-1,k))   for 1 < k < n.
   a(n) is the row‑major flattening of the triangular array.  */

/*  Common includes */
#include <stdint.h>   /* uint64_t, size_t */
#include <stdlib.h>   /* malloc, free, realloc */
#include <stdbool.h>  /* bool */
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

/*  Helper to compute lcm of two seq_t values. */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0) return (seq_t)0;
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/*  Find the row that contains position n (1‑based).  */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1)) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

/*  State kept between calls.  */
static idx_t cur_rows = 0;          /* number of rows already computed */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> tri_vec;   /* flattened triangle (C++ build) */
#else
static seq_t *tri_arr = NULL;        /* flattened triangle (C build) */
static size_t tri_len = 0;           /* allocated length of tri_arr */
#endif

/*  Main function – computes a(n).  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* sequence is defined for n ≥ 1 */

    /* Determine the row and column of the requested element. */
    idx_t row = find_row(n);
    idx_t col = n - (row - 1) * row / 2;   /* 1‑based column */

    /* Ensure rows up to 'row' are computed. */
    if (row > cur_rows) {

#ifdef __cplusplus
        /*  C++ implementation – use std::vector. */
        size_t new_len = (size_t)row * (row + 1ULL) / 2;
        if (new_len > tri_vec.size()) tri_vec.resize(new_len);

        /* Compute missing rows. */
        for (idx_t r = cur_rows + 1; r <= row; ++r) {
            size_t start = (size_t)(r - 1) * r / 2;
            size_t prev_start = start - (size_t)(r - 1);

            tri_vec[start] = (seq_t)r;
            tri_vec[start + r - 1] = (seq_t)r;

            for (idx_t k = 2; k <= r - 1; ++k) {
                seq_t left  = tri_vec[prev_start + k - 2];
                seq_t right = tri_vec[prev_start + k - 1];
                tri_vec[start + k - 1] = lcm_seq(left, right);
            }
        }
        cur_rows = row;
#else
        /*  C implementation – allocate with realloc. */
        size_t new_len = (size_t)row * (row + 1ULL) / 2;
        if (new_len > tri_len) {
            seq_t *tmp = (seq_t *)realloc(tri_arr, new_len * sizeof(seq_t));
            if (!tmp) {           /* allocation failure – return 0 */
                if (tri_arr) free(tri_arr);
                tri_arr = NULL;
                tri_len = 0;
                cur_rows = 0;
                return (seq_t)0;
            }
            tri_arr = tmp;
            tri_len = new_len;
        }

        /* Compute missing rows. */
        for (idx_t r = cur_rows + 1; r <= row; ++r) {
            size_t start = (size_t)(r - 1) * r / 2;
            size_t prev_start = start - (size_t)(r - 1);

            tri_arr[start] = (seq_t)r;
            tri_arr[start + r - 1] = (seq_t)r;

            for (idx_t k = 2; k <= r - 1; ++k) {
                seq_t left  = tri_arr[prev_start + k - 2];
                seq_t right = tri_arr[prev_start + k - 1];
                tri_arr[start + k - 1] = lcm_seq(left, right);
            }
        }
        cur_rows = row;
#endif
    }

    /* Retrieve the required element. */
    size_t index = (size_t)(row - 1) * row / 2 + (size_t)(col - 1);
#ifdef __cplusplus
    return tri_vec[index];
#else
    return tri_arr[index];
#endif
}
