/*
 * A153190  Triangle read by rows: t(n,m)=If[m == 0, 1, Product[m*k + 2, {k,
 *          0, n}]].
 * https://oeis.org/A153190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 34, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A153190
   Triangle read by rows:
   t(n,m) = If[m == 0, 1, Product[m*k + 2, {k, 0, n}]]
   Here a(n) is the anti‑diagonal enumeration of the triangle:
   a(n) = t(m, n - m*(m+1)/2),
   with m = floor( (sqrt(1+8*n)-1)/2 ).  The value m is the row
   (column in the triangle) that contains index n.  For m==0,
   all entries are 1.
*/

#include <stdint.h>

/* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2.  */
static inline idx_t find_m(idx_t n)
{
    idx_t low = 0, high = n, m = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 lhs = (unsigned __int128)mid * (mid + 1) / 2;
        if (lhs <= n) {
            m = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return m;
}

/* Main function required by the build system.  */
seq_t a(idx_t n)
{
    idx_t m = find_m(n);

    /* offset = m*(m+1)/2 */
    unsigned __int128 offset128 = (unsigned __int128)m * (m + 1) / 2;
    idx_t offset = (idx_t)offset128;
    idx_t inner = n - offset;               /* n in t(inner,m) */

    if (m == 0) {
        return (seq_t)1;                    /* t(k,0) == 1  */
    }

    seq_t result = 1;
    for (idx_t k = 0; k <= inner; ++k) {
        unsigned __int128 term_val = (unsigned __int128)m * k + 2;
        seq_t term = (seq_t)term_val;
        result *= term;
    }
    return result;
}
