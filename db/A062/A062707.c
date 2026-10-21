/*
 * A062707  Table by antidiagonals of n*k*(k+1)/2.
 * https://oeis.org/A062707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 4683, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antidiagonal enumeration of the table T(n,k)=n*k*(k+1)/2 (OEIS A062707)
   a(n): find the diagonal number d with d*(d+1)/2 <= n < (d+1)*(d+2)/2.
   offset = n - d*(d+1)/2
   let i = offset, j = d - offset
   then a(n) = i*j*(j+1)/2
   This matches the OEIS table listing by antidiagonals.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* binary search for diagonal d */
    idx_t low = 0, high = n;
    idx_t d = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= ( __int128 )n) {
            d = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* start index of diagonal d */
    __int128 start128 = (__int128)d * (d + 1) / 2;
    idx_t start = (idx_t)start128;          /* fits into idx_t */
    idx_t offset = n - start;               /* position within diagonal */

    idx_t i = offset;
    idx_t j = d - offset;

    seq_t result = (seq_t)i * (seq_t)j * ((seq_t)j + 1) / 2;
    return result;
}
