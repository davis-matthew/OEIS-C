/*
 * A000846  a(n) = C(3n,n) - C(2n,n).
 * https://oeis.org/A000846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n,n) - C(2n,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   binom_big, binom_fixed, seq_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_big.h"
#include "../utils/binom_fixed.h"
#include "../utils/seq_width.h"

seq_t a(idx_t n) {
    int w = seq_width();
    if (w >= 300)
        return binom_big(3 * n, n) - binom_big(2 * n, n);
    return (seq_t)(binom_fixed(3 * n, n, w) - binom_fixed(2 * n, n, w));
}
