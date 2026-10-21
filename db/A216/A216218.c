/*
 * A216218  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=2 or if
 *          k-n>=2, T(1,0) = T(0,0) = T(0,1) = 1, T(n,k) = T(n-1,k) +
 *          T(n,k-1).
 * https://oeis.org/A216218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the coordinates of n in the *
 *            antidiagonal enumeration of the square array T. For the array *
 *            T the recurrence is T(n,k)=T(n-1,k)+T(n,k-1) with T(n,k)=0 *
 *            when |n-k|>1 and T(0,0)=T(0,1)=T(1,0)=1. An explicit *
 *            expression is * T(n,k) = 0 if |n-k|>1 * T(n,k) = 2^min(n,...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root of a 128‑bit unsigned integer */
static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 ans = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* Find the diagonal s such that n < (s+1)(s+2)/2.
     * This satisfies s = floor((sqrt(8*n+1)-1)/2).  All calculations
     * are performed in 128‑bit to avoid overflow. */
    unsigned __int128 tmp   = (unsigned __int128)8 * n + 1;
    unsigned __int128 s128  = isqrt_u128(tmp);
    uint64_t s = (uint64_t)((s128 - 1) / 2);

    /* Pre‑count of elements in all previous diagonals */
    unsigned __int128 pre128 = (unsigned __int128)s * (s + 1) / 2;
    uint64_t pre = (uint64_t)pre128;          /* fits into 64‑bit */
    uint64_t offset = n - pre;                /* index inside diagonal */

    /* Coordinates in the array */
    uint64_t i = offset;
    uint64_t j = s - offset;

    /* Return either 0 or 2^min(i,j) */
    uint64_t diff = (i > j) ? i - j : j - i;
    if (diff > 1)
        return (seq_t)0;
    uint64_t minv = (i < j) ? i : j;
    return pow2(minv);
}
