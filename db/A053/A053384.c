/*
 * A053384  a(n) = A053398(4,n).
 * https://oeis.org/A053384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + A007814(k) with k = ceil(n/4), i.e. a(n) = 2 +
 *            (2-adic valuation of ceil(n/4)); pattern fitted to the listed
 *            terms
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 975 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = (n + 3) / 4;
    seq_t r = 2;
    while (k > 0 && (k & 1) == 0) {
        k >>= 1;
        r++;
    }
    return r;
}
