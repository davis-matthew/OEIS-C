/*
 * A186326  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the squares and octagonal
 *          numbers. Complement of A186327.
 * https://oeis.org/A186326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + max{k | k*(3k‑2) <= n^2}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence gives the rank of n² in the ordered merge of squares and
   octagonal numbers, with octagonal numbers ordered before the square
   when equal.  The function works for any n ≥ 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case (offset is 1, so n == 0 is unused). */
    if (n == 0) return 0;

    /* Compute n^2 in 128‑bit to avoid overflow for large n. */
    unsigned __int128 n128 = n;
    unsigned __int128 n2    = n128 * n128;

    /* Binary search for m = max{k | 3k^2 - 2k <= n^2}.
       Since 3k^2 >= k^2, we have k <= n, so the search range is [0, n]. */
    idx_t low  = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;          /* ceil midpoint */
        unsigned __int128 val = (unsigned __int128)3 * mid * mid
                               - (unsigned __int128)2 * mid;
        if (val <= n2) {
            low = mid;                                  /* possible solution */
        } else {
            high = mid - 1;                              /* too large */
        }
    }
    idx_t m = low;                                       /* count of octagons <= n^2 */

    /* The rank of n^2 is simply m + n (octagons are before the square). */
    seq_t result = (seq_t)m + (seq_t)n;
    return result;
}
