/*
 * A053121  Catalan triangle (with 0's) read by rows.
 * https://oeis.org/A053121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 233, u32 n <= 742, u64 n <= 2558,
 *            u128 n <= 9319, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catalan triangle a(n) – flattened by rows.  
   a(r,m) satisfies                           */
/*   a(0,0) = 1,  a(r,m)=0 for m>r or r+m odd   */
/*   a(r,m) = a(r-1,m-1) + a(r-1,m+1)           */
/* Then a(n) = a(r,m) where r is the unique row
   satisfying r(r+1)/2 ≤ n < (r+1)(r+2)/2 and
   m = n – r(r+1)/2.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r and column m in the flattened triangle. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t tri = r * (r + 1) / 2;
    idx_t m   = n - tri;

    /* For n up to about 10 000 the largest row is 141.
       Use small fixed-size static arrays to avoid dynamic
       allocation and to keep the code portable. */
    enum { MAX_R = 160 };
    seq_t prev[MAX_R] = {0};
    seq_t curr[MAX_R] = {0};

    /* Row 0 */
    prev[0] = 1;
    idx_t prev_len = 1;   /* length of previous row (r_prev+1) */

    /* Build rows 1..r by the recurrence */
    for (idx_t row = 1; row <= r; ++row)
    {
        /* Compute current row entries */
        for (idx_t col = 0; col <= row; ++col)
        {
            seq_t left  = (col == 0) ? 0 : prev[col - 1];
            seq_t right = (col + 1 < prev_len) ? prev[col + 1] : 0;

            /* Use 128‑bit arithmetic when available to avoid
               intermediate overflow; the conversion back to seq_t
               will naturally wrap if the value does not fit.      */
#if defined(__SIZEOF_INT128__)
            seq_t sum = (seq_t)((unsigned __int128)left + (unsigned __int128)right);
#else
            seq_t sum = left + right;
#endif
            curr[col] = sum;
        }

        /* Prepare for next iteration */
        for (idx_t col = 0; col <= row; ++col)
            prev[col] = curr[col];
        prev_len = row + 1;
    }

    /* Result is the entry at column m of row r */
    return prev[m];
}
