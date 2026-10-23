/*
 * A046741  Triangle read by rows giving number of arrangements of k dumbbells
 *          on 2 X n grid (n >= 0, k >= 0).
 * https://oeis.org/A046741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabl, changed
 * Formula:   a(n) is the n-th element of the triangle T(n,k), where * T(n,k)
 *            = T(n-1,k) + 2*T(n-1,k-1) + T(n-2,k-1) - T(n-3,k-3). * Base
 *            values: T(0,0)=1, T(1,0)=T(1,1)=1, T(2,0)=1, T(2,1)=4, T(2,2)=2.
 *            * The sequence a(n) is obtained by reading the triangle
 *            row‑by‑row
 * Range:     u8 n <= 24, u16 n <= 72, u32 n <= 242, u64 n <= 843,
 *            u128 n <= 3202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* Return the n‑th term of the flattened triangle T(n,k). */
seq_t a(idx_t n)
{
    /* Determine the row r and column k such that
     * r*(r+1)/2 <= n < (r+1)*(r+2)/2   and    k = n - r*(r+1)/2. */
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n )
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Handle the trivial rows directly. */
    if (r == 0) return (seq_t)1;
    if (r == 1) return (k == 0 || k == 1) ? (seq_t)1 : (seq_t)0;

    /* Build the triangle rows up to r. */
#ifdef __cplusplus
    /* C++: use std::vector to manage storage. */
    std::vector< std::vector<seq_t> > rows(r + 1);
    rows[0].resize(1);
    rows[0][0] = (seq_t)1;
    rows[1].resize(2);
    rows[1][0] = rows[1][1] = (seq_t)1;

    for (idx_t nrow = 2; nrow <= r; ++nrow) {
        rows[nrow].resize(nrow + 1);
        for (idx_t kk = 0; kk <= nrow; ++kk) {
            seq_t a_val = (kk <= nrow - 1) ? rows[nrow - 1][kk] : (seq_t)0;
            seq_t b_val = (kk >= 1) ? rows[nrow - 1][kk - 1] * (seq_t)2 : (seq_t)0;
            seq_t c_val = (kk >= 1 && kk - 1 <= nrow - 2) ? rows[nrow - 2][kk - 1] : (seq_t)0;
            seq_t d_val = (kk >= 3 && kk - 3 <= nrow - 3) ? rows[nrow - 3][kk - 3] : (seq_t)0;
            rows[nrow][kk] = a_val + b_val + c_val - d_val;
        }
    }
    return rows[r][k];
#else
    /* C: allocate rows with malloc/calloc. */
    seq_t **rows = (seq_t**)calloc((size_t)(r + 1), sizeof(seq_t*));
    if (!rows) return (seq_t)0;                  /* allocation fail */

    rows[0] = (seq_t*)calloc((size_t)1, sizeof(seq_t));
    if (!rows[0]) { free(rows); return (seq_t)0; }
    rows[0][0] = (seq_t)1;

    rows[1] = (seq_t*)calloc((size_t)2, sizeof(seq_t));
    if (!rows[1]) { free(rows[0]); free(rows); return (seq_t)0; }
    rows[1][0] = rows[1][1] = (seq_t)1;

    for (idx_t nrow = 2; nrow <= r; ++nrow) {
        rows[nrow] = (seq_t*)calloc((size_t)(nrow + 1), sizeof(seq_t));
        if (!rows[nrow]) {
            for (idx_t j = 0; j < nrow; ++j) free(rows[j]);
            free(rows);
            return (seq_t)0;
        }
        for (idx_t kk = 0; kk <= nrow; ++kk) {
            seq_t a_val = (kk <= nrow - 1) ? rows[nrow - 1][kk] : (seq_t)0;
            seq_t b_val = (kk >= 1) ? rows[nrow - 1][kk - 1] * (seq_t)2 : (seq_t)0;
            seq_t c_val = (kk >= 1 && kk - 1 <= nrow - 2) ? rows[nrow - 2][kk - 1] : (seq_t)0;
            seq_t d_val = (kk >= 3 && kk - 3 <= nrow - 3) ? rows[nrow - 3][kk - 3] : (seq_t)0;
            rows[nrow][kk] = a_val + b_val + c_val - d_val;
        }
    }

    seq_t result = rows[r][k];

    for (idx_t i = 0; i <= r; ++i) free(rows[i]);
    free(rows);
    return result;
#endif
}
