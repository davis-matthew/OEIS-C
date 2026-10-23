/*
 * A127449  Triangle T(n,k) = n if gcd(n,k)=1, =0 otherwise.
 * https://oeis.org/A127449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   a(n) is defined by the triangular array
   T(n,k) = n  if gcd(n,k)=1, 0 otherwise.
   a(n) is the n‑th entry of this array when read
   row by row (row 1: 1; row 2: 2,0; row 3: 3,3,0; …).

   Algorithm:
       1. Find the smallest r with r*(r+1)/2 ≥ n   (row number).
       2. k = n – r*(r-1)/2                         (column index).
       3. If gcd(r,k)=1 return r else return 0.

   The implementation uses only integer arithmetic; an integer
   square root is computed with a binary search.
*/

#include "../utils/gcd.h"

/* floor(sqrt(x)) for a 64‑bit unsigned integer, using binary search. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * mid <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* sequence A127449 */
seq_t a(idx_t n)
{
    if (n == 0)         /* safety: offset is 1 */
        return 0;

    /* 1. find row r : r*(r+1)/2 >= n */
    idx_t t = 1 + 8 * n;                      /* 8n+1 */
    idx_t s = floor_sqrt_u64(t);              /* floor(sqrt(8n+1)) */
    idx_t r = (s - 1) / 2;                    /* floor of the solution */
    /* adjust if still below n */
    if (( (__int128)r * (r + 1) / 2 ) < n)
        ++r;

    /* 2. compute column k */
    idx_t prev_tri = ( (__int128)r * (r - 1) / 2 );
    idx_t k = n - prev_tri;

    /* 3. gcd test */
    seq_t g = gcd((seq_t)r, (seq_t)k);
    return (g == (seq_t)1) ? (seq_t)r : (seq_t)0;
}
