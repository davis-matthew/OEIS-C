/*
 * A154537  Triangle T(n,m) read by rows: let p(n,x) = exp(-x) * Sum_{m >= 0}
 *          (2*m + 1)^n * x^m/m!; then T(n,m) = [x^m] p(n,x).
 * https://oeis.org/A154537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0,0)=1, T(n,0)=1,  
   T(n,k)=2*T(n-1,k-1)+(2*k+1)*T(n-1,k)  (1 ≤ k ≤ n).  
   The sequence a(n) is the n‑th element of the triangle read by rows
   (row 0, then row 1, …).                         */

#include <stdint.h>
#include <stddef.h>

#define MAX_R 200   /* enough for n up to about 20 000 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column index k in that row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Pre‑allocate storage for at most MAX_R+1 entries in a row */
    seq_t prev[MAX_R + 1] = {0};
    seq_t cur[MAX_R + 1]  = {0};

    prev[0] = (seq_t)1;          /* row 0 */

    if (r == 0)
        return prev[0];          /* a(0) */

    for (idx_t i = 1; i <= r; ++i) {
        cur[0] = (seq_t)1;        /* T(i,0)=1 */

        for (idx_t j = 1; j < i; ++j) {
            /* T(i,j)=2*T(i-1,j-1)+(2*j+1)*T(i-1,j) */
            cur[j] = (seq_t)2 * prev[j - 1]
                     + (seq_t)(2 * j + 1) * prev[j];
        }
        cur[i] = (seq_t)2 * prev[i - 1];   /* last entry, T(i,i) */

        /* copy current row into previous for next iteration */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = cur[j];
    }

    return prev[k];
}
