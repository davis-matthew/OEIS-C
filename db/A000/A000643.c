/*
 * A000643  a(n) = a(n-1) + 2^a(n-2).
 * https://oeis.org/A000643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2^a(n-2), a(0) = 0, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   a643_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a643_pow2.h"

seq_t a(idx_t n) {
    seq_t x = 0, y = 0; /* x = a(i-2), y = a(i-1) */
    idx_t i;
    if (n < 2) return 0;
    for (i = 2; i <= n; i++) {
        seq_t z = y + a643_pow2((idx_t)(unsigned long long)x);
        x = y;
        y = z;
    }
    return y;
}
