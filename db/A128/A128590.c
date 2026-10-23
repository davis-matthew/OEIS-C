/*
 * A128590  Triangle read by rows, matrix product A128179 * A000012.
 * https://oeis.org/A128590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained from the triangle T(r,k) = degree of
 *            q‑Fibonacci number F(r+2,k) with d(1,k)=0, d(2,k)=0,
 *            d(n,k)=max(d(n-1,k), d(n-2,k)+n+k-2) The triangle read by rows
 *            equals - For even rows r=2m: T(r,1)=T(r,2)=(m‑1+1)*(m+1),
 *            T(r,3)=T(r,4)=(m‑2+1)*(m+2), …
 *            T(r,(2p-1))=T(r,2p)=(m‑p+1)*(m+p), 1≤p...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard library headers for memory allocation */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that the first r rows contain at least n elements */
    idx_t r = 0;
    seq_t cum = 0;
    while (cum < n) {
        ++r;
        cum += (seq_t)r;            /* cum = r*(r+1)/2 without overflow */
    }

    /* Position in the current row (1‑based) */
    idx_t pos = (idx_t)(n - (cum - (seq_t)r)); /* 1 ≤ pos ≤ r */

    if ((r & 1) == 0) {              /* even row: r = 2m */
        idx_t m = r / 2;
        /* pair index: every two elements belong to the same pair */
        idx_t pair = (pos + 1) / 2;   /* ceil(pos/2) */
        seq_t val = (seq_t)(m - pair + 1) * (seq_t)(m + pair);
        return val;
    } else {                          /* odd row: r = 2m+1 */
        idx_t m = (r - 1) / 2;
        if (pos == 1) {               /* first unpaired element */
            seq_t val = (seq_t)(m + 1) * (seq_t)(m + 1);
            return val;
        } else {
            /* After the first element, positions 2,3 belong to pair 1,
               positions 4,5 to pair 2, etc. */
            idx_t pair = ((pos - 2) / 2) + 1;
            seq_t val = (seq_t)(m - pair + 1) * (seq_t)(m + pair + 1);
            return val;
        }
    }
}
