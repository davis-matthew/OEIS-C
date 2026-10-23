/*
 * A131844  3*A131821 - 2.
 * https://oeis.org/A131844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A131844
   A lower triangular array where row r has r+1 entries: two boundary
   entries equal to 3*r+1 and all inner entries equal to 1.
   The sequence enumerates the array in row-major order (row by row, left to right).

   Formula:
     Find the unique integer r such that
         r*(r+1)/2 <= n < (r+1)*(r+2)/2.
     Let c = n - r*(r+1)/2.
     Then
         a(n) = (c==0 || c==r) ? 3*r+1 : 1.
*/

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)). */
static unsigned __int128 floor_sqrt128(unsigned __int128 x)
{
    unsigned __int128 left = 0;
    unsigned __int128 right = ((unsigned __int128)1) << 64; /* exclusive upper bound */
    while (left < right) {
        unsigned __int128 mid = (left + right) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left - 1;
}

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2) */
    unsigned __int128 M = ((unsigned __int128)8) * n + 1;
    unsigned __int128 sqrtM = floor_sqrt128(M);
    idx_t r = (idx_t)((sqrtM - 1) / 2);

    /* Position within row r */
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;

    if (c == 0 || c == r)
        return (seq_t)(3 * r + 1);
    else
        return (seq_t)1;
}
