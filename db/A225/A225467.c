/*
 * A225467  Triangle read by rows, T(n, k) = 4^k*S_4(n, k) where S_m(n, k) are
 *          the Stirling-Frobenius subset numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 47, u64 n <= 140, u128 n <= 414,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) is defined by: 
   T(0,0) = 1; 
   T(n,k) = (3 + 4*k) * T(n-1,k) + 4 * T(n-1,k-1)  (for n ≥ 1, 0 ≤ k ≤ n). 
   The sequence a(n) is the triangle read by rows: 
   a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine the row r and column c that correspond to index n */
    idx_t r = 0;          /* current row index */
    idx_t cur = 0;        /* number of entries before row r */

    while (cur + r + 1 <= n) {
        cur += r + 1;
        ++r;
    }
    idx_t c = n - cur;    /* column within the found row */

    /* Allocate two rows of length r+1 */
    seq_t *prev = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        exit(1);   /* allocation failed */
    }

    /* Base case: row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r using the recurrence */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t k = 0; k <= i; ++k) {
            seq_t t_prev  = (k <= i - 1) ? prev[k] : (seq_t)0;
            seq_t t_prev2 = (k >= 1)     ? prev[k - 1] : (seq_t)0;
            seq_t mult    = (seq_t)(3 + 4 * k);
            curr[k] = mult * t_prev + (seq_t)4 * t_prev2;
        }
        /* swap prev and curr for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];

    /* free temporary storage */
    free(prev);
    free(curr);
    return result;
}
