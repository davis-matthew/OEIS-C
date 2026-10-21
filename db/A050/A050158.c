/*
 * A050158  T(n, k) = S(2*n + 1, n, k + 1) for 0<=k<=n and n >= 0, array S as
 *          in A050157.
 * https://oeis.org/A050158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = binomial(2*r+1, r+1) - binomial(2*r+1, r-k-1)
 *            where r = floor((sqrt(8*n+1)-1)/2) (row), k = n - r*(r+1)/2
 *            (column)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the OEIS A050158 sequence (flattened triangle).  */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Binary search to avoid overflow on large n. */
    idx_t low = 0;
    idx_t high = n;             /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 prod = (__int128)mid * (mid + 1) / 2;  /* safe product */
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    /* Compute k = n - r*(r+1)/2 */
    __int128 prod_r = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)prod_r;

    /* Compute T(r,k). */
    seq_t total1 = binomial(2 * r + 1, r + 1);
    seq_t total2 = binomial(2 * r + 1, r - k - 1);
    return total1 - total2;
}
