/*
 * A103994  Triangle read by rows: A129360 * A115361 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A103994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   a209635
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A103994: Triangle read by rows, entries T(r,k) = A209635(r/k) for k|r;
    a(n) is the nth element of that triangle (1‑based).  */
#include <stdint.h>
#include "../utils/a209635.h"

/* Locate the row r (1‑based) such that r(r+1)/2 ≥ n.  */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 1, high = 1;
    /* increase high until it is large enough */
    while ((__int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n ≤ 0 */

    idx_t r = row_of_index(n);
    idx_t tri_prev = r * (r - 1) / 2;        /* (r-1)r/2 */
    idx_t k = n - tri_prev;                  /* column within the row */

    if (r % k != 0) return (seq_t)0;         /* k does not divide r */

    idx_t m = r / k;
    return a209635(m);
}
