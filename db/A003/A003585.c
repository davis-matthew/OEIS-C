/*
 * A003585  Odd numbers that are not of the form x^2 + y^2 + 10*z^2.
 * https://oeis.org/A003585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   Odd numbers not representable as x^2 + y^2 + 10*z^2
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t isqrt_local(seq_t x) {
    if (x == 0) return 0;
    if (x == 1) return 1;
    
    seq_t s = x;
    while (1) {
        seq_t t = (s + x / s) / 2;
        if (t >= s) return s;
        s = t;
    }
}

static bool can_represent(seq_t n) {
    seq_t z_sq = 0;
    for (seq_t z = 0; ; z++) {
        seq_t contrib = (seq_t)10 * z_sq;
        if (contrib > n) break;
        
        seq_t remainder = n - contrib;
        for (seq_t x = 0; ; x++) {
            seq_t x_sq = x * x;
            if (x_sq > remainder) break;
            
            seq_t y_sq = remainder - x_sq;
            seq_t y = isqrt_local(y_sq);
            if (y * y == y_sq) {
                return true;
            }
        }
        z_sq = z_sq + (seq_t)2 * z + (seq_t)1;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 1;
    
    for (;;) {
        if (!can_represent(num)) {
            count++;
            if (count == n) {
                return num;
            }
        }
        num += 2;
    }
}
