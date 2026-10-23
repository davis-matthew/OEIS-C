/*
 * A122750  Triangle T(n,k) = (-1)^(k+1) if n is odd, = (-1)^k if n and k are
 *          even, = 2*(-1)^k if n is even and k is odd, 0<=k<=n.
 * https://oeis.org/A122750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122750: a(n) is the n‑th element when the triangle
   T(n,k) = (-1)^(k+1)                if n is odd
           = (-1)^k                  if n and k are even
           = 2*(-1)^k                if n is even and k is odd
   is read by rows.  The triangle has rows of length n+1, indexed
   with 0‑based row n and column k (0 ≤ k ≤ n).  For a given index n
   (0‑based) we first find the row r such that
      r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
   then let k = n - r*(r+1)/2, and return T(r,k). */

#include <stdint.h>

static inline seq_t triangle_value(idx_t r, idx_t k)
{
    if (r & 1)          /* r odd */
        return (k & 1) ? (seq_t)1 : (seq_t)-1;
    else                /* r even */
        return (k & 1) ? (seq_t)-2 : (seq_t)1;
}

seq_t a(idx_t n)
{
    /* Find r: the largest integer with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= ( __int128)n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Index of the first element in row r */
    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - start;

    return triangle_value(r, k);
}
