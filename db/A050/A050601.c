/*
 * A050601  Recursion counts for summation table A003056 with formula a(0,x) =
 *          x, a(y,0) = y, a(y,x) = a((y XOR x),2*(y AND x)).
 * https://oeis.org/A050601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   triangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/triangle.h"
/* Recursion counts for summation table A003056
   (OEIS A050601).  
   For each n ≥ 0, map n to the pair (y,x) given by the
   antidiagonal ordering of the infinite table:
          (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),…
   i.e. if k satisfies k(k+1)/2 ≤ n < (k+1)(k+2)/2, then
          d = n - k(k+1)/2,  y = d,  x = k - d.
   The value a(n) is the number of steps needed for the
   recursion
          a(y,x) = a(y XOR x, 2*(y AND x))
   until one coordinate becomes 0; at that point the recursion
   stops.  The loop below simulates this process directly using
   only integer operations. */

seq_t a(idx_t n)
{
    /* Find k such that triangle(k) ≤ n < triangle(k+1) */
    idx_t k = 0;
    while (triangle(k + 1) <= n)
        ++k;

    idx_t d = n - triangle(k);
    seq_t y = (seq_t)d;          /* first coordinate */
    seq_t x = (seq_t)(k - d);    /* second coordinate */

    seq_t steps = 0;
    while (x != 0 && y != 0) {
        seq_t tmp   = y ^ x;          /* new sum */
        seq_t carry = (y & x) << 1;   /* shifted carry */
        y = tmp;
        x = carry;
        steps = steps + 1;
    }
    return steps;
}
