/*
 * A132110  A007318 + A059268 - A000012 as infinite lower triangular matrices.
 * https://oeis.org/A132110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r, k) + 2^k - 1, * where r = floor((sqrt(8n+1)-1)/2)
 *            and k = n - r*(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = 1;
    /* Exponentially increase high until it is too large. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t tri = r * (r + 1) / 2;
    idx_t k = n - tri;

    seq_t part1 = binom(r, k);     /* C(r,k) */
    seq_t part2 = pow2(k);         /* 2^k */

    return part1 + part2 - 1;
}
