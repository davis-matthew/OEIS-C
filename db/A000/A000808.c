/*
 * A000808  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(2^(n+1)) + (2^n-1)*2^(2^n))/2^n = 2^(2^(n+1)-n) +
 *            (2^n-1)*2^(2^n-n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a808_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a808_pow2.h"

seq_t a(idx_t n) {
    idx_t e1 = ((idx_t)1 << (n + 1)) - n;
    idx_t e2 = ((idx_t)1 << n) - n;
    seq_t t1 = a808_pow2(e1);
    seq_t t2 = (a808_pow2(n) - 1) * a808_pow2(e2);
    return t1 + t2;
}
