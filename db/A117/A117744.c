/*
 * A117744  Triangle read by rows: coefficient of x^n in the power series of
 *          x/(1 - m*x - x^2 + x^3 - x^5) in row n, column m=1..n+2.
 * https://oeis.org/A117744
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117744: coefficient of x^n in x/(1 - m*x - x^2 + x^3 - x^5) where
   row r has length r+2 and m = column index (1‑based).  The linear
   index n is defined with a(-1)=0, then rows start at
   S(r) = r*(r+3)/2 (row 0 starts at 0). */

#include <stdint.h>

#define MAX_ROW 10010   /* enough for n up to about 10000 */

seq_t a(idx_t n)
{
    if (n < 0)                /* a(-1) is defined as 0 */
        return 0;

    /* Find the row r containing index n:  S(r) <= n < S(r+1)  */
    idx_t r = 0;
    while ( ((r + 1) * (r + 4)) / 2 <= n )
        ++r;

    /* Starting linear index of row r */
    idx_t row_start = r * (r + 3) / 2;
    idx_t offset    = n - row_start;            /* 0‑based offset in the row */
    seq_t m         = (seq_t)(offset + 1);      /* m = column, 1‑based */

    /* Compute the coefficient b_{r-1} of the reciprocal series
       B(x)=1/(1-mx-x^2+x^3-x^5).  For r==0 all entries are 0.   */
    if (r == 0)
        return 0;

    static seq_t b[MAX_ROW];
    b[0] = 1;                         /* b_0 = 1 */

    if (r > 1) {
        idx_t k;
        for (k = 1; k <= r - 1; ++k) {
            seq_t val = m * b[k - 1];
            if (k >= 2) val += b[k - 2];
            if (k >= 3) val -= b[k - 3];
            if (k >= 5) val += b[k - 5];
            b[k] = val;
        }
    }

    return b[r - 1];
}
