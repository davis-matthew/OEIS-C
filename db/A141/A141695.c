/*
 * A141695  Triangle T(n,m) = A120070(n,m)-A143814(n,m) read by rows, 1<=m<n.
 * https://oeis.org/A141695
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = 2*m+1 if m < r-1, otherwise -(r-2)*r, where r is the
 *            smallest integer ≥2 such that r*(r-1)/2 > n-2, and m = n-2 -
 *            (r-1)*(r-2)/2 + 1
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes r by binary search (O(log n))
   and returns the requested triangle entry. */

#include <stdint.h>

/* convert between idx_t and seq_t is done via cast (as per requirement) */

static seq_t
compute_a(idx_t n)
{
    /* n >= 2 */
    idx_t t = n - 2;                /* zero‑based offset within the triangle */

    /* locate the row r by binary search on r(r-1)/2 > t */
    idx_t low = 2, high = 2;

    /* double high until it exceeds the needed bound */
    while ((unsigned __int128)high * (high - 1) / 2 <= t)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 > t)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                  /* r >= 2 */

    /* position inside the row */
    idx_t prev = (r - 1) * (r - 2) / 2;    /* elements before row r */
    idx_t offset = t - prev;               /* zero‑based within row r */
    idx_t m = offset + 1;                  /* column index (1 ≤ m < r) */

    /* compute the triangle entry */
    if (m == r - 1) {                     /* last entry of the row */
        return -((seq_t)(r - 2) * (seq_t)r);
    } else {
        return (seq_t)(2 * (seq_t)m + 1);
    }
}

/* external linkage as requested */
seq_t a(idx_t n)
{
    return compute_a(n);
}
