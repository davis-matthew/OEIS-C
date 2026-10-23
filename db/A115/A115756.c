/*
 * A115756  Triangle where a(1,1)=1. a(n,m) = number of terms among first
 *          (n-1) terms of sequence A115757 which are coprime to m. A115757(n)
 *          is the sum of terms in the n-th row of triangle A115756.
 * https://oeis.org/A115756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element of triangle A115756 at row r, column c where r =
 *            smallest integer with r*(r+1)/2 >= n, c = n - (r-1)*r/2. The
 *            triangle entry a(r,c) counts the number of terms among the first
 *            r-1 terms of A115757 that are coprime to c
 * Range:     u8 n <= 1092, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Header requirements */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

#ifdef __cplusplus
#include <vector>
#endif

/* triangular number (r*(r+1)/2) */
static inline idx_t triangular(idx_t x) { return x * (x + 1) / 2; }

/* Find the row r such that triangular(r-1) < n <= triangular(r) */
static idx_t row_of_index(idx_t n)
{
    idx_t r = 1;
    while (triangular(r) < n) ++r;
    return r;
}

/* --------------------------------------------------------------- */
/* Storage for generated rows                                      */
/* --------------------------------------------------------------- */

#ifdef __cplusplus
/* C++ build: use std::vector, which performs proper construction/destruction */
static std::vector<seq_t> row_sums;     /* row_sums[i] = A115757(i), i>=1 */
static std::vector<seq_t> flat;        /* flattened triangle, index 0-based */
static idx_t max_row = 0;
#else
/* C build: use raw malloc/realloc; seq_t is a plain integer type */
static seq_t *row_sums = NULL;         /* row_sums[i] = A115757(i), 1<=i<=max_row */
static seq_t *flat      = NULL;        /* flattened triangle entries */
static idx_t max_row   = 0;
#endif

/* Ensure all rows up to and including r are computed */
static void ensure_capacity(idx_t r)
{
    if (r <= max_row) return;

#ifdef __cplusplus
    /* --- C++ implementation --- */
    row_sums.resize(r + 1);          /* indices 0..r, we ignore 0 */
    flat.resize(triangular(r));     /* need storage for all entries */

    for (idx_t i = max_row + 1; i <= r; ++i) {
        seq_t row_sum = 0;
        idx_t offset = triangular(i - 1); /* first index of row i in flat */

        if (i == 1) {                    /* base case */
            flat[0] = (seq_t)1;
            row_sums[1] = (seq_t)1;
            continue;
        }

        for (idx_t m = 1; m <= i; ++m) {
            seq_t cnt = 0;
            for (idx_t k = 1; k < i; ++k) {
                if (gcd(row_sums[k], (seq_t)m) == (seq_t)1) ++cnt;
            }
            flat[offset + m - 1] = cnt;
            row_sum += cnt;
        }
        row_sums[i] = row_sum;
    }
    max_row = r;
#else
    /* --- C implementation --- */
    /* Resize row_sums array */
    seq_t *new_row_sums = (seq_t *)realloc(row_sums, (r + 1) * sizeof(seq_t));
    if (!new_row_sums) exit(EXIT_FAILURE);
    row_sums = new_row_sums;

    /* Resize flat array */
    size_t new_flat_size = (size_t)triangular(r);
    seq_t *new_flat = (seq_t *)realloc(flat, new_flat_size * sizeof(seq_t));
    if (!new_flat) exit(EXIT_FAILURE);
    flat = new_flat;

    for (idx_t i = max_row + 1; i <= r; ++i) {
        seq_t row_sum = 0;
        idx_t offset = triangular(i - 1); /* first index of row i in flat */

        if (i == 1) {                    /* base case */
            flat[0] = (seq_t)1;
            row_sums[1] = (seq_t)1;
            continue;
        }

        for (idx_t m = 1; m <= i; ++m) {
            seq_t cnt = 0;
            for (idx_t k = 1; k < i; ++k) {
                if (gcd(row_sums[k], (seq_t)m) == (seq_t)1) ++cnt;
            }
            flat[offset + m - 1] = cnt;
            row_sum += cnt;
        }
        row_sums[i] = row_sum;
    }
    max_row = r;
#endif
}

/* --------------------------------------------------------------- */
/* Public function                                                 */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, not used */

    idx_t r = row_of_index(n);
    ensure_capacity(r);
    return flat[n - 1];
}
