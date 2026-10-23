/*
 * A015123  Triangle of q-binomial coefficients for q=-10.
 * https://oeis.org/A015123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,c) where r = largest integer with r(r+1)/2 ≤ n, c = n
 *            – r(r+1)/2; T(n,0)=T(n,n)=1 and for 0<k<n:
 *            T(n,k)=T(n-1,k-1)+(-10)^k*T(n-1,k)
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 29, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t start = (r * (r + 1)) / 2;
    idx_t c = n - start;

    /* Allocate two rows of length r+1 */
    seq_t *prev = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failure – abort */
        abort();
    }

    /* Row 0 */
    prev[0] = 1;

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = 1;                 /* T(i,0) = 1 */
        seq_t power = (seq_t)-10;    /* (-10)^1 */

        for (idx_t j = 1; j <= i; ++j) {
            if (j == i) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = prev[j - 1] + power * prev[j];
                power = power * (seq_t)-10;
            }
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];

    free(prev);
    free(curr);
    return result;
}
