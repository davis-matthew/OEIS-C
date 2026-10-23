/*
 * A169614  Triangular array: T(n,k)=integer nearest F(n)/F(n-k),
 *          k=1,2,...,n-2; n>=3, where F=A000045 (Fibonacci numbers).
 * https://oeis.org/A169614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A169614.
   In the triangular array T(n,k)= nearest integer of F(n)/F(n-k) (F = Fibonacci).
   Rows are indexed by r = 1,2,3,...
   Row r contains r terms:  T(r+2,k) for k = 1..r.
   The sequence a(1),a(2),... is obtained by reading the rows left‑to‑right.
   For a given index n we locate the row r with  (r-1)*r/2 < n ≤ r*(r+1)/2
   and column k = n - (r-1)*r/2.  Then a(n) = nearest( F(r+2)/F(r+2-k) ). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that (r-1)*r/2 < n <= r*(r+1)/2  */
    idx_t r = 1;
    for (;; ++r) {
        unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
        if (tri >= n) break;
    }
    /* previous cumulative: (r-1)*r/2 = tri - r  (from the loop) */
    idx_t k = n - ((unsigned __int128)r * (r - 1) / 2);  /* 1‑based column */

    /* Prepare Fibonacci numbers up to index m = r + 2.  Only the values
       at m and at m-k are needed.  Compute them in a single forward pass. */
    idx_t m = r + 2;
    seq_t top = 0, bot = 0;
    seq_t a0 = 0, a1 = 1;   /* Fibonacci F0, F1 */

    /* Handle the trivial cases */
    if (m == 0) {  /* not reachable for r>=1 */
        top = 0; bot = 1;
    } else if (m == 1) {
        top = 1; bot = (m-k == 1) ? 1 : 0;
    } else {
        /* iterate i = 2..m */
        for (idx_t i = 2; i <= m; ++i) {
            seq_t a2 = a0 + a1;  /* F(i) = F(i-2)+F(i-1) */
            a0 = a1;
            a1 = a2;
            if (i == m - k) bot = a1;
            if (i == m)     top = a1;
        }
    }

    /* nearest integer of top/bot */
    seq_t q = top / bot;
    seq_t rem = top % bot;
    if (rem * 2 >= bot) ++q;
    return q;
}
