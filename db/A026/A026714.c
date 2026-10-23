/*
 * A026714  Triangular array T read by rows: T(n,0)=T(n,n)=1 for n >= 0; for n
 *          >= 2 and 1<=k<=n-1, T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k) if k=[
 *          (n-1)/2 ] or k=[ n/2 ] or k=[ (n+2)/2 ], else
 *          T(n,k)=T(n-1,k-1)+T(n-1,k).
 * https://oeis.org/A026714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026714:  Triangular array T read by rows.
   Recurrence:
     T(0,0) = 1,
     For n ≥ 1: T(n,0) = T(n,n) = 1.
     For n ≥ 2, 1 ≤ k ≤ n-1,
         if k==⌊(n-1)/2⌋ or k==⌊n/2⌋ or k==⌊(n+2)/2⌋
              T(n,k) = T(n-1,k-1)+T(n-2,k-1)+T(n-1,k);
         else
              T(n,k) = T(n-1,k-1)+T(n-1,k).
   a(n) is the n‑th element when the rows of T are read left‑to‑right, top‑to‑bottom.
   For 1‑based index n, let r be the row such that r(r+1)/2 < n ≤ (r+1)(r+2)/2
   and let k = n-1-r(r+1)/2 (0‑based column). Then a(n)=T(r,k). */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Determine the row r and column k corresponding to index n. */
    idx_t r = 0;
    idx_t cum = 0;            /* elements in rows 0 … r-1 */
    while (cum + r + 1 < n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - 1 - cum;    /* 0‑based column in row r */

    /* The maximum possible row for n up to ~10000 is < 200,
       so a fixed 200×200 table is more than enough. */
    seq_t T[200][200];

    /* Base row 0 */
    T[0][0] = (seq_t)1;
    if (r == 0) return T[0][0];

    /* Base row 1 */
    T[1][0] = T[1][1] = (seq_t)1;
    if (r == 1) return T[1][k];

    /* Compute rows up to r */
    for (idx_t i = 2; i <= r; ++i) {
        T[i][0]          = (seq_t)1;
        T[i][i]          = (seq_t)1;
        idx_t half1      = (i - 1) / 2;
        idx_t half2      = i / 2;
        idx_t half3      = (i + 2) / 2;

        for (idx_t j = 1; j < i; ++j) {
            seq_t val = T[i-1][j-1] + T[i-1][j];
            if (j == half1 || j == half2 || j == half3) {
                val += T[i-2][j-1];
            }
            T[i][j] = val;
        }
    }

    return T[r][k];
}
