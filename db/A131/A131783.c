/*
 * A131783  A000012 * (A004736 + A002260 - I).
 * https://oeis.org/A131783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((r+1)(r+2) - (j+1)(j+2))/2 + j where n = r(r-1)/2 + j
 *            (row r, column j, 1‑based)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for uint64_t or larger. */
static idx_t int_sqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) return 0;
        if (mid <= x / mid) {          /* mid*mid <= x  */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n >= 1 (for correct use); guard against n==0 */
    if (n == 0) return 0;

    /* find the row r such that r(r-1)/2 < n <= r(r+1)/2  */
    idx_t val   = 8 * n + 1;              /* 8*n+1 fits in idx_t for n up to 10^19 */
    idx_t sqrtv = int_sqrt_idx(val);      /* floor(sqrt(8*n+1)) */
    idx_t r = (sqrtv - 1) / 2;            /* lower bound of the row   */
    while ((r * (r + 1)) / 2 < n) ++r;    /* adjust to true row */

    /* column j (1‑based) */
    idx_t j = n - (r * (r - 1)) / 2;

    /* compute a(r,j) = ((r+1)(r+2) - (j+1)(j+2))/2 + j  */
    seq_t Rp1 = (seq_t)(r + 1);
    seq_t Rp2 = (seq_t)(r + 2);
    seq_t temp = Rp1 * Rp2;                /* (r+1)(r+2) */

    seq_t Jp1 = (seq_t)(j + 1);
    seq_t Jp2 = (seq_t)(j + 2);
    seq_t sub = Jp1 * Jp2;                 /* (j+1)(j+2) */

    seq_t result = (temp - sub) / 2 + (seq_t)j;
    return result;
}
