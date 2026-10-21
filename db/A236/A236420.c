/*
 * A236420  Riordan array (1/(1-3*x), (1-3*x-sqrt(1-6*x+5*x^2))/(2*x)).
 * https://oeis.org/A236420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = nth element of the triangle T with T(n,0) = 3^n, T(n,k) =
 *            T(n-1,k-1) + 3*T(n-1,k) + T(n-1,k+1) for k>0, reading the rows
 *            left‑to‑right (rows 0,1,2,…)
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the required value for any n ≥ 0
   in O(√n) time with a small, fixed amount of memory. */

#include <stdlib.h>

#define MAX_ROW 2000 /* sufficient for n up to about 10,000 */

static seq_t the_prev[MAX_ROW + 1];
static seq_t the_curr[MAX_ROW + 1];

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the entry a(n). */
    idx_t r = 0;
    /* Find largest r with r*(r+1)/2 ≤ n */
    while ((idx_t)(((__int128)(r + 1) * (r + 2) / 2)) <= n)
        ++r;
    idx_t t = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - t;

    /* Compute the triangle up to row r. */
    seq_t *prev = the_prev;
    seq_t *curr = the_curr;

    /* Row 0 */
    prev[0] = 1;
    seq_t pow3 = 1;   /* 3^0 */

    for (idx_t row = 1; row <= r; ++row) {
        pow3 = pow3 * 3;   /* 3^row */
        curr[0] = pow3;

        for (idx_t col = 1; col <= row; ++col) {
            seq_t a = prev[col - 1];
            seq_t b = (col <= row - 1) ? prev[col] : 0;
            seq_t c = (col + 1 <= row - 1) ? prev[col + 1] : 0;
            curr[col] = a + 3 * b + c;
        }
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    return prev[k];
}
