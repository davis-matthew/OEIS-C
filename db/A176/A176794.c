/*
 * A176794  Triangle read by rows: T(n, k) = 2^n*(q^k - 1)*(q^(n - k) - 1) +
 *          1, where q = 3.
 * https://oeis.org/A176794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^r * (3^k - 1) * (3^(r-k) - 1) + 1, where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2. This is the
 *            triangle read by rows of OEIS A176794
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 91, u64 n <= 325,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* compute k = n - r*(r+1)/2 safely */
    unsigned __int128 r_base128 = (__int128)r * (r + 1) / 2;
    idx_t r_base = (idx_t)r_base128;
    idx_t k = n - r_base;

    /* compute the triangle entry */
    seq_t pow2_r   = pow2(r);
    seq_t pow3_k   = pow3(k);
    seq_t pow3_rk  = pow3(r - k);

    seq_t term     = (pow3_k - 1) * (pow3_rk - 1);
    seq_t result   = pow2_r * term + 1;

    return result;
}
