/*
 * A154986  Triangle T(n, k) = coefficients of p(x, n) where p(x,n) =
 *          (x+1)*p(x, n-1) + n*(n-1)*x*p(x, n-2), read by rows.
 * https://oeis.org/A154986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) satisfies * T(n,0) = T(n,n) = 1 *
 *            T(n,k) = T(n-1,k) + T(n-1,k-1) + n*(n-1)*T(n-2,k-1) * The
 *            sequence a(n) is obtained by writing the triangle * row‑by‑row
 *            (row major order). For an index n we find the * row r and column
 *            k such that * r(r+1)/2 ≤ n < (r+1)(r+2)/2 * and k...
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 95, u64 n <= 219,
 *            u128 n <= 609, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Maximum row needed for any n ≤ 10000 :
 * (r+1)(r+2)/2 > 10000  →  r ≈ 141.  Provide some margin. */
#define MAX_ROW 200

/* Static storage for the triangle values */
static seq_t T[MAX_ROW + 1][MAX_ROW + 1];

seq_t a(idx_t n)
{
    /* Find row r such that S(r-1) ≤ n < S(r), where
     * S(r) = (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1U) * (r + 2U)) / 2U <= n)
        ++r;

    /* Column k within row r. */
    idx_t k = n - r * (r + 1U) / 2U;

    /* Compute the triangle up to row r. */
    T[0][0] = (seq_t)1;
    if (r == 0)
        return T[0][0];

    T[1][0] = (seq_t)1;
    T[1][1] = (seq_t)1;

    for (idx_t i = 2; i <= r; ++i)
    {
        T[i][0] = (seq_t)1;
        T[i][i] = (seq_t)1;
        seq_t mult = (seq_t)i * (seq_t)(i - 1U);

        for (idx_t j = 1; j < i; ++j)
        {
            /* T(i,j) = T(i-1,j) + T(i-1,j-1) + i*(i-1)*T(i-2,j-1) */
            T[i][j] = T[i - 1][j] + T[i - 1][j - 1] + mult * T[i - 2][j - 1];
        }
    }

    return T[r][k];
}
