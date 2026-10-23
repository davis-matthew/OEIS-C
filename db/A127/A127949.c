/*
 * A127949  A000012 as an infinite lower triangular matrix with all 1's;
 *          A127899 = a simple transform; then A000012 * A127899. Given
 *          A051340, change all 1's to -1. Triangle read by rows, (n-1) -1's
 *          followed by "n".
 * https://oeis.org/A127949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = n, if n is a triangular number (n = r*(r+1)/2 for some
 *            integer r ≥ 1); otherwise a(n) = -1
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses an integer binary search for r. No floating point is used. */

#include <stdint.h>

/* Return a(n) for all n ≥ 1.  For n that is not a triangular number this
   function returns -1.  The implementation works for any idx_t and seq_t
   provided by the build, including arbitrary‑precision integer types. */
seq_t a(idx_t n)
{
    /* Protect zero index (offset 1 in OEIS). */
    if (n == 0)
        return (seq_t)-1;

    /* Binary search for r such that r*(r+1)/2 == n.
       Use unsigned __int128 for intermediate products to avoid overflow. */
    idx_t low = 0;
    idx_t high = n;               /* r cannot exceed n for n ≥ 1 */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;

        if (tri == n)
            return (seq_t)mid;    /* triangular number found */
        else if (tri < n)
            low = mid + 1;
        else
            high = (mid == 0) ? 0 : mid - 1;  /* avoid underflow */
    }

    /* n is not a triangular number */
    return (seq_t)-1;
}
