/*
 * A051623  a(n) = Sum_{x=1+floor(sqrt(n))..floor(sqrt(2n))} (x^2 - n).
 * https://oeis.org/A051623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{x = 1 + floor(sqrt(n)) .. floor(sqrt(2n))} (x² - n)
 * Range:     u8 n <= 112, u16 n <= 4801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9798 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* integer square‑roots */
    idx_t lo = isqrt(n);        /* ⌊√n⌋       */
    idx_t hi = isqrt(n + n);    /* ⌊√(2n)⌋    */

    seq_t sum = 0;
    if (lo + 1 <= hi) {
        for (idx_t x = lo + 1; x <= hi; ++x) {
            seq_t sq = (seq_t)x * (seq_t)x;
            sum += sq - (seq_t)n;
        }
    }
    return sum;
}
