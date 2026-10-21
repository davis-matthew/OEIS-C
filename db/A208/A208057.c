/*
 * A208057  Triangle by rows, generated from the odd integers and related to
 *          A000165.
 * https://oeis.org/A208057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle A208057:  inverse of the lower triangular matrix
    M(n,k) = -(2k+1) for k < n,  M(n,n) = 1.
    The entries T(n,k) satisfy
        T(n,n) = 1,
        T(n,k) = Σ_{i=k}^{n-1} (2i+1) * T(i,k)   for k < n.
    The flattened sequence a(n) is obtained by traversing the triangle
    row by row (row 0: 1; row 1: 1,1; row 2: 4,3,1; ...).
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row r and column c such that n = r*(r+1)/2 + c   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t c = n - r * (r + 1) / 2;

    /* static cache of the triangle up to the largest row
       that has been computed so far                                 */
    static seq_t *cache = NULL;
    static idx_t rows_computed = 0;     /* number of complete rows; 0 means nothing computed */
    static size_t cache_capacity = 0;   /* number of seq_t elements cached               */

    /* extend cache if necessary */
    if (r >= rows_computed) {
        idx_t new_rows = r + 1;
        size_t new_cap = (size_t)new_rows * (new_rows + 1) / 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */

        /* compute missing rows */
        for (idx_t row = rows_computed; row <= r; ++row) {
            idx_t start = row * (row + 1) / 2;   /* first index of this row in buffer */
            if (row == 0) {
                cache[0] = 1;
                continue;
            }
            for (idx_t col = 0; col < row; ++col) {
                seq_t sum = 0;
                for (idx_t i = col; i < row; ++i) {
                    idx_t idx_i_col = i * (i + 1) / 2 + col;
                    sum += ((seq_t)(2 * i + 1)) * cache[idx_i_col];
                }
                cache[start + col] = sum;
            }
            cache[start + row] = 1;   /* diagonal element */
        }
        rows_computed = new_rows;
        cache_capacity = new_cap;
    }

    /* return requested element */
    size_t idx = (size_t)(r * (r + 1) / 2 + c);
    return cache[idx];
}
