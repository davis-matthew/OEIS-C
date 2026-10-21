/*
 * A059434  Triangle formed when cumulative boustrophedon transform is applied
 *          to 1, 1, 1, 1, ..., read by rows in natural order.
 * https://oeis.org/A059434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) comes from the cumulative boustrophedon transform *
 *            triangle T(i,j) defined by * * T(i,0) = 1 (i ≥ 0) * T(i,1) =
 *            T(i,0) + T(i-1,i-1) (i ≥ 1) * T(i,j) = 2·T(i,j-1) – T(i,j-2) +
 *            T(i-1,i-j) (i ≥ 2, j ≥ 2, j ≤ i) * * a(n) = T(i,j) where i and j
 *            are chosen so that the * row‑wise reading of the tr...
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"

/* Integer floor square‑root for a 128‑bit unsigned integer.
 * The return type is idx_t (64‑bit). */
static idx_t int_sqrt_u128(unsigned __int128 x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 35;          /* high > sqrt(2^68) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Cached triangle rows.  triangle[i] points to an array of length i+1
 * holding the values T(i,0) … T(i,i). */
static idx_t   max_row = (idx_t)-1;   /* not yet initialised */
static seq_t **triangle = NULL;       /* array of row pointers */

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Find row i and column j such that a(n) = T(i,j). */
    unsigned __int128 val      = (unsigned __int128)8 * n + 1;
    idx_t            i         = (int_sqrt_u128(val) - 1) / 2;
    idx_t            row_start = i * (i + 1) / 2;
    idx_t            j         = n - row_start;

    /* Initialise storage on first call. */
    if (max_row == (idx_t)-1) {
        max_row = 0;
        triangle = (seq_t**)malloc(sizeof(seq_t*));
        triangle[0] = seq_alloc_new_array(1);        /* row 0 has one element */
        triangle[0][0] = (seq_t)1;
    }

    /* Extend the triangle up to the needed row i. */
    while (max_row < i) {
        idx_t new_row = max_row + 1;

        /* Re‑allocate the pointer array to hold one more row. */
        triangle = (seq_t**)realloc(triangle,
                                    (size_t)(new_row + 1) * sizeof(seq_t*));
        triangle[new_row] = seq_alloc_new_array((size_t)(new_row + 1));

        /* T(new_row,0) = 1. */
        triangle[new_row][0] = (seq_t)1;

        /* T(new_row,1) = T(new_row,0) + T(new_row-1,new_row-1). */
        if (new_row >= 1) {
            triangle[new_row][1] =
                triangle[new_row][0] + triangle[new_row-1][new_row-1];
        }

        /* General recurrence for j >= 2. */
        for (idx_t col = 2; col <= new_row; ++col) {
            triangle[new_row][col] =
                2 * triangle[new_row][col-1] -
                triangle[new_row][col-2] +
                triangle[new_row-1][new_row-col];
        }
        ++max_row;
    }

    return triangle[i][j];
}
