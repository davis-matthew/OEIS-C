/*
 * A216238  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=1 or if
 *          k-n>=5, T(0,0) = T(0,1) = T(0,2) = T(0,3) = T(0,4) = 1, T(n,k) =
 *          T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T by antidiagonals.
 *            The array T is defined by T(0,k) = 1 for 0<=k<=4 (otherwise 0)
 *            T(n,k) = 0 if n-k>=1 or k-n>=5 T(n,k) = T(n-1,k) + T(n,k-1) for
 *            other (n,k) Here a(n) = T(i,j) where i+j = d and i = n -
 *            d*(d+1)/2, j = d-i. The function below computes...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Static table for the array T.
       T[ row * 5 + offset ]  (offset = j - i , 0 <= offset <= 4). */
    static seq_t *T = NULL;
    static idx_t T_rows = 0;       /* number of rows currently stored (>=1) */

    /* Find the diagonal d for index n:  d = floor((sqrt(8n+1)-1)/2). */
    double s = sqrt((double)(8ULL * n + 1ULL));
    idx_t d = (idx_t)((s - 1.0) * 0.5);

    /* Correct possible rounding errors. */
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;
    while (d * (d + 1) / 2 > n)
        --d;

    /* Make sure we have T computed up to row d (i <= d). */
    if (T_rows <= d) {
        idx_t new_rows = d + 1;
        size_t len = (size_t)new_rows * 5;
        seq_t *new_T = (seq_t*)malloc(len * sizeof(seq_t));
        if (!new_T) return (seq_t)0;            /* allocation failure */

        /* Initialise all entries to zero. */
        for (size_t i = 0; i < len; ++i)
            new_T[i] = 0;

        /* Row 0: T(0,k)=1 for 0<=k<=4. */
        for (int off = 0; off < 5; ++off)
            new_T[off] = (seq_t)1;

        /* Remaining rows. */
        for (idx_t i = 1; i < new_rows; ++i) {
            for (int off = 0; off < 5; ++off) {
                seq_t upper = (off + 1 <= 4) ? new_T[(i - 1) * 5 + off + 1] : 0;
                seq_t left  = (off - 1 >= 0) ? new_T[i * 5 + off - 1] : 0;
                new_T[i * 5 + off] = upper + left;
            }
        }

        /* Replace old table. */
        free(T);
        T = new_T;
        T_rows = new_rows;
    }

    /* Compute (i,j) for this index. */
    idx_t i = n - d * (d + 1) / 2;
    idx_t j = d - i;

    /* Offsetting value j-i must lie between 0 and 4. */
    int diff = (int)(j - i);
    if (diff < 0 || diff > 4)
        return (seq_t)0;

    return T[i * 5 + diff];
}
