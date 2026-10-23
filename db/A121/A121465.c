/*
 * A121465  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n such that the sum of the heights of their
 *          triangles is k (0<=k<=n). A triangle in a Dyck path is a subpath
 *          of the form U^h D^h, starting at the x-axis; here U=(1,1),
 *          D=(1,-1), h being the height of the triangle.
 * https://oeis.org/A121465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = floor((sqrt(8*n+1)-1)/2) * c = n -
 *            r(r+1)/2 * T(r,0) = fib(2r-3)-1 * T(r,c) (c>=1) =
 *            2^(c-1)*(fib(2r-2c-3)-1) * (fib is the Fibonacci function from
 *            the utils library)
 * Range:     u8 n <= 44, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"  /* provides seq_t fib(idx_t n); */
#include "../utils/pow2.h" /* provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    while ( (unsigned __int128)high * (high + 1) / 2 <= n )
        high <<= 1;               /* double until we exceed n */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ( (unsigned __int128)mid * (mid + 1) / 2 <= n )
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    /* Column c within that row */
    idx_t row_start = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - row_start;

    /* T(r,0) uses fibonacci(2r-3)-1 */
    int64_t idx0 = (int64_t)2 * r - 3;
    seq_t fib0;
    if (idx0 >= 0)
        fib0 = fib((idx_t)idx0);
    else
        fib0 = fib((idx_t)(-idx0));  /* idx0 will be -1 or -3 only */

    seq_t val0 = fib0 - 1;
    if (c == 0)
        return val0;

    /* For c >= 1 : 2^(c-1) * (fib(2r-2c-3)-1) */
    int64_t idxk = (int64_t)2 * r - (int64_t)2 * c - 3;
    seq_t fibk;
    if (idxk >= 0)
        fibk = fib((idx_t)idxk);
    else
        fibk = fib((idx_t)(-idxk));  /* again -1 or -3 */

    seq_t valk = fibk - 1;
    seq_t factor = pow2(c - 1);      /* c >= 1 here */
    return factor * valk;
}
