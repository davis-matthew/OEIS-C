/*
 * A135425  a(n) = 6^n * 7^(n^2).
 * https://oeis.org/A135425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n * 7^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* 6^n */
    seq_t p6 = seq_pow((seq_t)6, n);
    /* exponent for 7: n^2, safe for n up to 10^4 in 64‑bit */
    idx_t exp7 = n * n;
    seq_t p7 = seq_pow((seq_t)7, exp7);
    /* product */
    return seq_mul(p6, p7);
}
