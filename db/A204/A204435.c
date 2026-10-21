/*
 * A204435  Symmetric matrix: f(i,j)=((i+j)^2 mod 3), read by (constant)
 *          antidiagonals.
 * https://oeis.org/A204435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Symmetric matrix sequence:  f(i,j) = (i+j)^2 mod 3 (0 or 1).  
   Enumerate the entries by antidiagonals (constant i+j).  
   a(n) = 1 if the antidiagonal number s = (k+1) is not divisible by 3,
          0 otherwise, where k is the smallest integer such that
          k*(k+1)/2 >= n.  (k is the index of the antidiagonal with
          prefix length >= n.) 
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find minimal k with k*(k+1)/2 >= n */
    idx_t k, low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    k = low;                   /* k >= 1 for n >= 1 */

    idx_t s = k + 1;           /* antidiagonal number */

    /* return 1 iff s mod 3 != 0 */
    return (seq_t)((s % 3) ? 1 : 0);
}
