/*
 * A080818  Triangle read by rows in which the n-th row contains n distinct
 *          numbers whose sum is the smallest square.
 * https://oeis.org/A080818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080818: Triangle read by rows where the n‑th row contains n distinct
 * numbers whose sum is the smallest possible square.
 * For the r‑th row the first r‑1 elements are 1,2,…,r‑1.
 * The last element v is the smallest integer > r‑1 such that
 * (t + v) is a perfect square, where t = r(r-1)/2.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* find the row r that contains the n-th element */
    idx_t r = 0;
    idx_t cum = 0;                /* cumulative number of elements so far */
    while (cum < n) {
        r++;
        cum += r;
    }

    idx_t prev_cum = cum - r;     /* elements before row r */
    idx_t pos = n - prev_cum;     /* 1‑based position inside row r */

    /* sum of the first (r-1) natural numbers */
    seq_t t = (seq_t)r * (seq_t)(r - 1) / (seq_t)2;

    if (pos < r)                   /* one of the first r-1 elements */
        return (seq_t)pos;

    /* smallest square ≥ t + r */
    seq_t target = t + (seq_t)r;
    seq_t s_floor = floor_sqrt(target);
    seq_t s = s_floor * s_floor;
    if (s < target) {
        s_floor++;
        s = s_floor * s_floor;
    }

    /* the r‑th element */
    return s - t;
}
