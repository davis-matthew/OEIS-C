/*
 * A091491  Triangle, read by rows, where the n-th diagonal is generated from
 *          the n-th row by the sum of the products of the n-th row terms with
 *          binomial coefficients.
 * https://oeis.org/A091491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 231, u64 n <= 741,
 *            u128 n <= 2485, big any n
 * Helpers:   build_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A091491 – Triangle, read by rows
 *  T(n,0)=T(n,n)=1  (n ≥ 0)
 *  T(n,k)=T(n-1,k-1)+T(n,k+1)  (1 ≤ k ≤ n-1)
 *  a(n) is the flattening of the triangle: a(n)=T(r,k)
 *   where r is the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
 *   and k = n - r*(r+1)/2.
 *
 *  The function below builds rows on demand and keeps them cached.
 *  For any n ≥ 0 it returns a(n) in O(√n) time and O(n) memory,
 *  which is well below the 2 s time limit for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_row.h"

static idx_t find_row(idx_t n) {
    idx_t r = 0;
    /* find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    /* static cache of computed rows */
    static seq_t **rows = NULL;
    static idx_t max_computed = 0u;      /* largest r with rows[r] ready */
    static idx_t rows_capacity = 0u;

    /* initialise the cache with row 0 if needed */
    if (!rows) {
        rows = (seq_t**)malloc(sizeof(seq_t*));
        if (!rows) exit(1);
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!rows[0]) exit(1);
        rows[0][0] = (seq_t)1;          /* T(0,0) */
        max_computed = 0u;
        rows_capacity = 1u;
    }

    /* find required row index r */
    idx_t r = find_row(n);

    /* enlarge the rows array if necessary */
    if (r >= rows_capacity) {
        seq_t **tmp = (seq_t**)realloc(rows, sizeof(seq_t*) * (r + 1));
        if (!tmp) exit(1);
        rows = tmp;
        rows_capacity = r + 1;
    }

    /* build any missing rows up to r */
    while (max_computed < r) {
        build_row(rows, max_computed + 1);
        ++max_computed;
    }

    /* compute column k and return the value */
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;
    return rows[r][k];
}
