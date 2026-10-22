/*
 * A000824  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with b_n(r,s) = (2^(r*2^n) + (2^n-1)*2^(s*2^(n-1)))/2^n, a(n) =
 *            (b_n(3,3) + 3*b_n(2,3) + 2*b_n(1,1))/6. Equivalently, with X =
 *            2^n: 3*a(n) = 2^(3X-1-n) + 3*2^(2X-1-n) + 2^(X-n) +
 *            (X-1)*(2^(3X/2+1-n) + 2^(X/2-n))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a824_fixed, a824_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a824_fixed.h"
#include "../utils/a824_pow2.h"

seq_t a(idx_t n) {
    int fixed = a824_fixed();
    uint64_t X, h;
    seq_t s;
    if (fixed && n > 7) return 0; /* value cannot fit any fixed width */
    X = (uint64_t)1 << n;
    h = X / 2;
    s = a824_pow2(3 * X - 1 - n);
    s += 3 * a824_pow2(2 * X - 1 - n);
    s += a824_pow2(X - n);
    s += (seq_t)(X - 1) * (a824_pow2(3 * h + 1 - n) + a824_pow2(h - n));
    if (fixed) {
        /* exact division by 3 modulo 2^w via inverse of 3 */
        seq_t inv = 3;
        int i;
        for (i = 0; i < 8; i++) inv = inv * (seq_t)(2 - 3 * inv);
        return s * inv;
    }
    return s / 3;
}
