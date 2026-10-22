/*
 * A000809  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(3*2^n)+(2^n-1)*2^(3*2^(n-1)))/2^n = 2^(3*2^n-n) +
 *            2^(3*2^(n-1)) - 2^(3*2^(n-1)-n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a000809_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000809_p2.h"

seq_t a(idx_t n) {
    if (n < 1 || n > 40) return 0;
    idx_t p = (idx_t)1 << n;
    idx_t h = p >> 1;
    seq_t t1 = a000809_p2(3 * p - n);
    seq_t t2 = a000809_p2(3 * h);
    seq_t t3 = a000809_p2(3 * h - n);
    seq_t r = t1;
    r += t2;
    r -= t3;
    return r;
}
