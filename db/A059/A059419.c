/*
 * A059419  Triangle T(n,k) (1 <= k <= n) of tangent numbers, read by rows:
 *          T(n,k) = coefficient of x^n/n! in expansion of (tan x)^k/k!.
 * https://oeis.org/A059419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = T(r,k) where r is the unique integer with r(r‑1)/2 < n ≤
 *            r(r+1)/2, * k = n – r(r‑1)/2. * Recurrence for the triangle T: *
 *            T(1,1) = 1 * T(r,r) = 1 * T(r+1,k) = T(r,k‑1) + k*(k+1)*T(r,k+1)
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Convert linear index n to (row r, column k) – both 1‑based */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1.0) / 2.0);
    while ((r * (r + 1)) / 2 < n) ++r;
    while (r > 0 && ((r - 1) * r) / 2 >= n) --r;
    idx_t k = n - ((r - 1) * r) / 2;   /* 1 ≤ k ≤ r */

    /* Cached triangle: T[row][col] with row and col 1‑based, row 0 unused */
    static seq_t **T = NULL;
    static idx_t maxRow = 0;

    if (maxRow < r) {
        /* Grow pointer array */
        T = (seq_t**)realloc(T, (r + 1) * sizeof(seq_t*));
        if (T == NULL) return (seq_t)0;   /* should never happen */

        /* Allocate new rows and compute them */
        for (idx_t row = maxRow + 1; row <= r; ++row) {
            T[row] = (seq_t*)malloc((row + 1) * sizeof(seq_t));   /* index 0 unused */
            if (T[row] == NULL) return (seq_t)0;                 /* should never happen */
            for (idx_t col = 1; col <= row; ++col) {
                if (col == row) {
                    T[row][col] = (seq_t)1;
                } else {
                    seq_t left  = (col > 1) ? T[row - 1][col - 1] : (seq_t)0;
                    seq_t right = (col + 1 <= row - 1) ? T[row - 1][col + 1] : (seq_t)0;
                    seq_t mul   = (seq_t)col * (seq_t)(col + 1) * right;
                    T[row][col] = left + mul;
                }
            }
        }
        maxRow = r;
    }

    return T[r][k];
}
