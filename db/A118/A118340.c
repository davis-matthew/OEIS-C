/*
 * A118340  Pendular triangle, read by rows, where row n is formed from row
 *          n-1 by the recurrence: if n > 2k, T(n,k) = T(n,n-k) + T(n-1,k),
 *          else T(n,k) = T(n,n-1-k) + T(n-1,k), for n>=k>0, with T(n,0) = 1
 *          and T(n,n) = 0^n.
 * https://oeis.org/A118340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 126, u32 n <= 389, u64 n <= 1247,
 *            u128 n <= 4322, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Pendular triangle A118340.
 *  The triangle is built row by row using pendular sums of the previous row.
 *  Recurrence:
 *     T(n,0)=1 , T(n,n)=0 (for n>0)
 *     For 0<k<n:
 *        if n>2k : T(n,k)=T(n,n-k)+T(n-1,k)
 *        else    : T(n,k)=T(n,n-1-k)+T(n-1,k)
 *  The sequence a(n) is obtained by reading the triangle in order of rows.
 *  a(n) = T(r,k) where r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *  and k = n - r(r+1)/2.
 */

#include <stddef.h>
#include <math.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* ----- Find the row that contains index n -------------------------------- */
    /* r is the largest integer such that r(r+1)/2 ≤ n. */
    idx_t r = (idx_t)((sqrt((long double)8 * (long double)n + 1) - 1) / 2);

    /* Correct possible rounding errors.  Use 128‑bit arithmetic to avoid overflow. */
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k     = n - start;          /* column within the row */

    /* ----- Allocate two buffers for the current and previous rows ----------- */
    seq_t *buf1 = alloc_seq_array(r + 1);
    seq_t *buf2 = alloc_seq_array(r + 1);
    if (!buf1 || !buf2) {
        if (buf1) free_seq_array(buf1);
        if (buf2) free_seq_array(buf2);
        return (seq_t)0;
    }

    /* Row 0 */
    buf1[0] = (seq_t)1;                /* T(0,0)=1 */
    seq_t *prev = buf1;
    seq_t *curr = buf2;

    /* ----- Build rows 1 … r ---------------------------------------------- */
    for (idx_t row = 1; row <= r; ++row) {
        idx_t left  = 0;
        idx_t right = (idx_t)(row - 1);   /* previous row has length 'row' */
        seq_t sum   = (seq_t)0;
        int turn    = 0;                 /* 0 → use left, 1 → use right */

        while (left <= right) {
            idx_t i = (turn == 0) ? left++ : right--;
            sum += prev[i];
            curr[i] = sum;
            turn ^= 1;
        }

        curr[row] = (seq_t)0;            /* T(row,row) = 0 for row>0 */

        /* Swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];  /* value in row r, column k */

    /* Free temporary buffers */
    free_seq_array(buf1);
    free_seq_array(buf2);

    return result;
}
