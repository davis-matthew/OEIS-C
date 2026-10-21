/*
 * A086915  Triangle read by rows: T(n,k) = 2^k * (n!/k!)*binomial(n-1,k-1).
 * https://oeis.org/A086915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 67, u64 n <= 173,
 *            u128 n <= 499, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A086915
   a(n) is the nth element of the triangle read by rows:
        T(r,k) = 2^k * (r!/k!) * binomial(r-1,k-1),
   where r and k are obtained from n by
        r = smallest integer with r*(r+1)/2 >= n
        k = n - (r-1)*r/2.
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

static seq_t term_T(idx_t r, idx_t k)
{
    seq_t res = 1;

    /* multiply by 2^k */
    for (idx_t i = 0; i < k; ++i)
        res *= (seq_t)2;

    /* multiply by r! / k!  = product i = k+1 .. r  */
    for (idx_t i = k + 1; i <= r; ++i)
        res *= (seq_t)i;

    /* multiply by binomial(r-1,k-1) */
    res *= binom(r - 1, k - 1);

    return res;
}

seq_t a(idx_t N)
{
    if (N < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* binary search for row r */
    idx_t low = 1, high = N, ans = N;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit intermediate to avoid overflow */
        unsigned __int128 cnt = (unsigned __int128)mid * (mid + 1) / 2;
        if (cnt >= N) {
            ans = mid;
            if (mid == 0) break;  /* avoid infinite loop if N==0 */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = ans;

    /* number of elements before row r */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = N - (idx_t)prev;

    return term_T(r, k);
}
