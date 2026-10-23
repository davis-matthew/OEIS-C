/*
 * A125165  Triangle read by rows: T(n,k) = C(n,k) + 3*C(n,k+1) + 2*C(n,k+2)
 *          (0<=k<=n).
 * https://oeis.org/A125165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * T(r,k) = C(r,k) + 3*C(r,k+1) + 2*C(r,k+2)
 *            * with r = floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 37, u16 n <= 141, u32 n <= 573, u64 n <= 2174,
 *            u128 n <= 5150, big any n
 * Helpers:   binom_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_128.h"

seq_t a(idx_t n)
{
    /* find row r: offset(r) = r*(r+1)/2 <= n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    seq_t value = binom_128(r, k);
    if (k + 1 <= r) {
        value += (seq_t)3 * binom_128(r, k + 1);
    }
    if (k + 2 <= r) {
        value += (seq_t)2 * binom_128(r, k + 2);
    }
    return value;
}
