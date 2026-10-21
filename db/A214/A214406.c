/*
 * A214406  Triangle of second-order Eulerian numbers of type B.
 * https://oeis.org/A214406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2 *
 *            and k = n - r(r+1)/2. * T(n,k) is defined recursively by *
 *            T(0,0) = 1, * T(n,k) = (4*n - 2*k - 1) * T(n-1,k-1) * + (2*k +
 *            1) * T(n-1,k) for 0 <= k <= n, n>0
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
static inline idx_t find_row(idx_t n) {
    idx_t low = 0, high = n + 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) low = mid + 1;
        else high = mid;
    }
    return low - 1;
}

/* Triangle storage (pointer to each row). */
static seq_t **triangle = NULL;
static size_t triangle_cap = 0;
static idx_t max_row_computed = 0;

/* Initialise the triangle with row 0. */
static void init_triangle(void) {
    if (triangle) return;
    triangle_cap = 4;
    triangle = (seq_t**)malloc(triangle_cap * sizeof(seq_t*));
    if (!triangle) abort();
    for (size_t i = 0; i < triangle_cap; ++i) triangle[i] = NULL;
    seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
    row0[0] = (seq_t)1;
    triangle[0] = row0;
    max_row_computed = 0;
}

/* Ensure that rows up to r are computed. */
static void ensure_row(idx_t r) {
    init_triangle();
    if (r <= max_row_computed) return;
    if ((size_t)r >= triangle_cap) {
        size_t new_cap = triangle_cap;
        while ((size_t)r >= new_cap) new_cap *= 2;
        triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
        for (size_t i = triangle_cap; i < new_cap; ++i) triangle[i] = NULL;
        triangle_cap = new_cap;
    }
    for (idx_t n = max_row_computed + 1; n <= r; ++n) {
        seq_t *prev = triangle[n - 1];
        seq_t *row = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = prev[0];
            } else if (k == n) {
                idx_t coeff = 4 * n - 2 * k - 1;
                val = (seq_t)coeff * prev[k - 1];
            } else {
                idx_t coeff1 = 4 * n - 2 * k - 1;
                idx_t coeff2 = 2 * k + 1;
                val = (seq_t)coeff1 * prev[k - 1] + (seq_t)coeff2 * prev[k];
            }
            row[k] = val;
        }
        triangle[n] = row;
    }
    max_row_computed = r;
}

/* Main sequence generation function. */
seq_t a(idx_t n) {
    idx_t row = find_row(n);
    unsigned __int128 tri128 = (unsigned __int128)row * (row + 1) / 2;
    idx_t tri = (idx_t)tri128;
    idx_t col = n - tri;
    ensure_row(row);
    return triangle[row][col];
}
