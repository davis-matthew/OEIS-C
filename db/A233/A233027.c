/*
 * A233027  a(n) is number of nonnegative integers not exceeding n which have
 *          no decimal digits occurring in n and respectively prime to n.
 * https://oeis.org/A233027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k <= n with gcd(k, n) = 1 and digit(k) ∩
 *            digit(n) = ∅
 * Range:     u8 n <= 503, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return a bit mask of the decimal digits present in v.
   Bit d is set iff digit d appears in v. */
static inline uint16_t
digit_mask(uint64_t v)
{
    uint16_t mask = 0;
    if (v == 0) {
        mask |= 1 << 0;
    } else {
        while (v) {
            mask |= 1 << (v % 10);
            v /= 10;
        }
    }
    return mask;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;
    uint16_t mask_n = digit_mask(n);
    seq_t cnt = 0;
    seq_t sn = (seq_t)n;          /* reuse for gcd */
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, sn) == 1) {
            if ((digit_mask(k) & mask_n) == 0) {
                ++cnt;
            }
        }
    }
    return cnt;
}
