/*
 * A185917  Weight array of max{n,k}, by antidiagonals.
 * https://oeis.org/A185917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the nth element of the array T(n,k) listed by
 *            antidiagonals, where T(n,k) = 1 if n==1 or k==1 T(n,k) = -1 if
 *            n==k (>1) T(n,k) = 0 otherwise. The sequence starts with a(1)=1.
 *            This implementation computes (n,k) for a given n and returns the
 *            corresponding value using O(log n) arithmetic in 1...
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t and related types */

seq_t a(idx_t n)
{
    /* Find the diagonal s (sum n+k) containing the nth element.
       For diagonal s, there are s-1 elements.
       The total number of elements up to diagonal s is (s-1)*s/2. */
    idx_t low = 2, high = 3;
    /* Grow high until it surely exceeds the required diagonal */
    while ((unsigned __int128)(high - 1) * high / 2 < (unsigned __int128)n) {
        low = high;
        high <<= 1;
    }

    /* Binary search for minimal s with (s-1)*s/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)(mid - 1) * mid / 2;
        if (tri >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;   /* diagonal index (sum of coordinates) */

    /* Elements before this diagonal */
    unsigned __int128 prev_tri = (unsigned __int128)(s - 2) * (s - 1) / 2;
    /* Position within the diagonal (0‑based) */
    idx_t offset = (idx_t)(((unsigned __int128)n) - prev_tri - 1);

    /* Coordinates in the array:
       i increases from 1 upward, j decreases from s-1 downward. */
    idx_t i = offset + 1;
    idx_t j = (s - 1) - offset;

    /* Return the array value according to the definition of T. */
    if (i == 1 || j == 1)
        return 1;
    else if (i == j) /* i==j>1 automatically */
        return -1;
    else
        return 0;
}
