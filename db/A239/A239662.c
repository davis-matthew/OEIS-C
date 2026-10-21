/*
 * A239662  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the numbers A017113 interleaved with k-1 zeros, and the first
 *          element of column k is in row k(k+1)/2.
 * https://oeis.org/A239662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the flattened triangle T where
 *            T(r,k)=8*((r - k(k+1)/2)/k +1)-4 if (r - k(k+1)/2)%k==0, else 0;
 *            the triangle is read row‑by‑row
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   row_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/row_length.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    idx_t r = 1;
    idx_t cum = 0;

    /* find the row r such that the nth term lies in row r                   */
    while (1) {
        idx_t len = row_length(r);
        if (cum + len >= n)
            break;
        cum += len;
        ++r;
    }

    idx_t pos   = n - cum - 1;   /* 0‑based position within the row      */
    idx_t k     = pos + 1;       /* 1‑based column index                */

    /* first row containing column k */
    idx_t start = k * (k + 1) / 2;
    idx_t diff  = r - start;

    if (diff % k == 0) {
        idx_t t = diff / k + 1;    /* index in A017113 (1‑based)           */
        return (seq_t)((unsigned __int128)8 * t - 4);
    } else {
        return (seq_t)0;
    }
}
