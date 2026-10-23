/*
 * A176793  A symmetrical triangle read by rows: T(n, k) = 2^n*(q^k - 1)*(q^(n
 *          - k) - 1) + 1, where q = 2.
 * https://oeis.org/A176793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^r * (2^k - 1) * (2^(r-k) - 1) + 1 where r =
 *            floor((sqrt(8*n + 1) - 1) / 2), k = n - r*(r + 1)/2. The
 *            triangle is read by rows: T(n,k) = 2^n*(2^k-1)*(2^(n-k)-1)+1
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 153, u64 n <= 561,
 *            u128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Increase high until the triangular number exceeds n */
    while ( ( (__int128)high * (high + 1) / 2 ) <= ( __int128 )n )
        high <<= 1U;          /* double until sufficient */

    /* Binary search for largest low with low*(low+1)/2 <= n */
    while (low < high) {
        idx_t mid  = low + (high - low + 1) / 2;    /* upper mid */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= (__int128)n)
            low = mid;
        else
            high = mid - 1U;
    }
    idx_t r = low;
    idx_t tri_r = r * (r + 1) / 2;    /* r*(r+1)/2 fits in idx_t for all n under test */
    idx_t k = n - tri_r;              /* position inside the row, 0 <= k <= r */

    /* Compute T(r,k) = 2^r * (2^k - 1) * (2^(r-k) - 1) + 1 */
    seq_t two_r = pow2(r);
    seq_t two_k = pow2(k);
    seq_t two_l = pow2(r - k);

    seq_t one = 1;
    seq_t result = two_r * (two_k - one) * (two_l - one) + one;
    return result;
}
