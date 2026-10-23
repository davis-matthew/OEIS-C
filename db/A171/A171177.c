/*
 * A171177  Triangle read by rows in which row n lists 3n-1 together with the
 *          first 2n-1 numbers <> 0 of A038608, in reverse order.
 * https://oeis.org/A171177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = 3*row - 1 if n is the first element of the row a(n) =
 *            (-1)^k * k otherwise, where row = smallest r >= 1 such that
 *            r*(r+1) >= n and k = 2*row - pos + 1 with pos = n - (row-1)*row
 *            (1 <= pos <= 2*row)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* find the row containing position n */
    idx_t lo = 1;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (( (__int128)mid * (mid + 1) ) >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    idx_t row = lo;                /* row index (>=1) */

    /* position inside the row */
    idx_t prev = (idx_t)(( (__int128)(row - 1) * row ));   /* terms before this row */
    idx_t pos = n - prev;                          /* 1 <= pos <= 2*row */

    if (pos == 1) {                               /* first term of the row */
        return (seq_t)(3 * row - 1);
    } else {
        /* index k of the non‑zero term of A038608 (in normal order) */
        idx_t k = 2 * row - pos + 1;              /* 1 <= k <= 2*row-1 */
        seq_t val = (seq_t)k;                     /* magnitude */
        if (k & 1) {                             /* odd k gives negative sign */
            return -val;
        } else {
            return val;
        }
    }
}
