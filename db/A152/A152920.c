/*
 * A152920  Triangle read by rows: triangle A062111 reversed.
 * https://oeis.org/A152920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            r*(r+1)/2 * T(r,0) = r * T(r,k) = (2*r – k) * 2^(k-1) (k ≥ 1)
 * Range:     u8 n <= 33, u16 n <= 118, u32 n <= 463, u64 n <= 1889,
 *            u128 n <= 7748, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find r = floor((sqrt(8*n+1)-1)/2) using integer arithmetic. */
    unsigned __int128 target = (unsigned __int128)8 * n + 1;   /* = 8*n+1 */
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 34);                            /* > sqrt(target) */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;                    /* upper mid */
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;                                            /* floor(sqrt) */
    idx_t r = (s - 1) >> 1;                                   /* row index */

    /* Compute the starting linear index of row r. */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;

    /* Column within the row. */
    idx_t k = n - start;                                      /* 0 ≤ k ≤ r */

    if (k == 0)                                               /* T(r,0) = r */
        return (seq_t)r;

    seq_t coeff = (seq_t)(2 * r - k);                         /* (2*r-k) */
    seq_t shift = pow2(k - 1);                                /* 2^(k-1) */
    return coeff * shift;                                     /* (2*r-k)*2^(k-1) */
}
