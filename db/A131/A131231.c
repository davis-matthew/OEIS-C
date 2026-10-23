/*
 * A131231  3*A130296 - 2*A128174.
 * https://oeis.org/A131231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major entry of a lower triangular matrix defined
 *            by * T(n,1) = first column: numbers congruent to {0,1} mod 6,
 *            i.e. * T(2k+1,1) = 6k+1, T(2k+2,1) = 6k+6 * T(n,j) = 1 if
 *            parity(n) == parity(j), else 3 for j ≥ 2 * a(n) = T(r,c) where
 *            r,c are the row and column of the index n * (row‑...
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n ≤ T(r), where T(r)=r*(r+1)/2 */
    idx_t r = 0;
    if (n > 0) {
        long double d = sqrtl((long double)(8.0L * (long double)n + 1.0L));
        r = (idx_t)((d - 1.0L) * 0.5L);
        /* Adjust in case of rounding errors */
        while (( (__uint128_t)r * (r + 1)) / 2 < n) ++r;
        while (r > 0 && (( (__uint128_t)(r - 1) * r) / 2) >= n) --r;
    }

    /* Compute previous triangle size T(r-1) */
    idx_t prev = (r > 0) ? ( ( (__uint128_t)(r - 1) * r) / 2 ) : 0;
    idx_t c = n - prev;  /* column index (1‑based) */

    /* First column values */
    if (c == 1) {
        if (r % 2 == 1) {         /* r = 2k+1 */
            return (seq_t)(6 * ((r - 1) / 2) + 1);
        } else {                  /* r = 2k   */
            return (seq_t)(6 * (r / 2));
        }
    }

    /* Remaining entries are 1 or 3 depending on parity of row and column */
    return ( ( (r & 1) == (c & 1) ) ? (seq_t)1 : (seq_t)3 );
}
