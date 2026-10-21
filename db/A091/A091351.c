/*
 * A091351  Triangle T, read by rows, such that T(n,k) equals the (n-k)-th row
 *          sum of T^k, where T^k is the k-th power of T as a lower triangular
 *          matrix.
 * https://oeis.org/A091351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T row‑wise. The
 *            triangle is defined by T(n,0) = 1 (n ≥ 0) T(n,k) = Σ_{j=0}^{n-k}
 *            T(n-k,j) * T(j+k-1,k-1) (k≥1, n≥k). The linear index n is mapped
 *            to a row r and column k as r is the largest integer with
 *            r(r+1)/2 ≤ n, k = n – r(r+1)/2. Thus a(n) = T(r,k)...
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 121, u64 n <= 300,
 *            u128 n <= 705, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of computed rows */
static seq_t **T = NULL;
static idx_t rows = 0;          /* number of rows currently stored */

/* ensure that rows 0 .. r are computed and stored in T */
static void ensure_rows(idx_t r)
{
    idx_t i;

    if (r < rows)
        return;                 /* already available */

    /* resize row pointer array */
    T = (seq_t **)realloc(T, sizeof(seq_t *) * (r + 1));
    /* compute missing rows one by one */
    for (i = rows; i <= r; ++i) {
        /* allocate current row: i+1 elements (columns 0..i) */
        T[i] = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        T[i][0] = (seq_t)1;        /* T(i,0) = 1 */
        /* compute columns 1 .. i */
        if (i > 0) {
            idx_t kk;
            for (kk = 1; kk <= i; ++kk) {
                seq_t sum = (seq_t)0;
                idx_t j_max = i - kk;
                idx_t j;
                for (j = 0; j <= j_max; ++j) {
                    sum += T[i - kk][j] * T[j + kk - 1][kk - 1];
                }
                T[i][kk] = sum;
            }
        }
    }
    rows = r + 1;                 /* now we have rows 0..r */
}

seq_t a(idx_t n)
{
    /* determine row r and column k corresponding to index n */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* make sure the triangle is built up to row r */
    ensure_rows(r);

    /* return the required element */
    return T[r][k];
}
