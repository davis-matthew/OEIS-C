/*
 * A143084  Triangle read by rows: T(n,m) = (n + m)!.
 * https://oeis.org/A143084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 75, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A143084.
   A143084 is the triangle read by rows with entries T(n,m) = (n+m)!.
   The sequence a(n) is obtained by flattening this triangle row by row:
   a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), ...
   For a given index n, let r be the row number such that
     r(r+1)/2 <= n < (r+1)(r+2)/2
   and let m = n - r(r+1)/2.  Then a(n) = (r+m)!.
*/

seq_t a(idx_t n)
{
    /* Determine the row index r. */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n) {
        r++;
    }
    r--;   /* last r that satisfies the inequality */

    /* Column offset within the row. */
    idx_t m = n - (r * (r + 1)) / 2;

    /* Value to factorialise: k = r + m. */
    idx_t k = r + m;

    /* Compute k! iteratively. */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
