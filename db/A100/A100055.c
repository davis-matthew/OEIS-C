/*
 * A100055  Where records occur in A100053.
 * https://oeis.org/A100055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n-3) for n >= 9; a(0) = 0, a(1) = 2, a(2) = 4, a(3) =
 *            6, a(4) = 8, a(5) = 16, a(6) = 32, a(7) = 43, a(8) = 46
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper to compute 2^exp in type seq_t, assuming exp fits in seq_t's width */
static seq_t pow2(idx_t exp) {
    seq_t r = 1;
    while (exp) {
        r <<= 1;
        exp--;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 4;
    if (n == 3) return 6;
    if (n == 4) return 8;
    if (n == 5) return 16;
    if (n == 6) return 32;
    if (n == 7) return 43;
    if (n == 8) return 46;
    return pow2(n - 3);
}
