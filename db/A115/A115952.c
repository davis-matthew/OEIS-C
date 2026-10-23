/*
 * A115952  Expansion of (1-x+x*y)/(1-x^2*y^2) - x^2/(1-x^2*y).
 * https://oeis.org/A115952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 5150, i16 n <= 5150, i32 n <= 5150, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115952: flattening the triangle T(n,k)
   T(n,k) = 1          if k == n
          = -1         if k == n-1 and k even
          = -1         if n even and k == (n-2)/2
          =  0         otherwise
   For a given index n, find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
   Then k = n - r(r+1)/2 and a(n) = T(r,k). */

#include <stdint.h>

/* Find the row r for index n: maximum r with r(r+1)/2 <= n. */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;                    /* r cannot exceed n */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        /* use 128‑bit to avoid overflow of mid*(mid+1) */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main function for the OEIS sequence */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);

    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;     /* safe because start <= n */
    idx_t k = n - start;               /* column index within the row */

    /* Triangle definition */
    if (k == r) {
        return (seq_t)1;
    }
    if (r > 0 && k == r - 1) {
        /* k even -> -1, k odd -> 0 */
        return ((k & 1u) == 0u) ? (seq_t)-1 : (seq_t)0;
    }
    if ((r & 1u) == 0u && r >= 2) {
        idx_t kv = (r - 2) / 2;
        if (k == kv) {
            return (seq_t)-1;
        }
    }
    return (seq_t)0;
}
