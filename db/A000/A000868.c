/*
 * A000868  Number of switching networks with C(2,n) acting on domain and
 *          GL(2,Z2) acting on range.
 * https://oeis.org/A000868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(2^(n+1)) + 2^(n+2 + 2^n) - 2^(2^n) +
 *            2^(n+1))/(3*2^(n+1)). With N = 2^n and m = (2^N - 1)/3 this
 *            simplifies to a(n) = m*2^(N-n-1) + 2*m + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a868_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a868_width.h"

seq_t a(idx_t n) {
    static int w = 0;
    if (w == 0) w = a868_width();
    if (n < 1 || n >= 63) return 0;
    idx_t N = (idx_t)1 << n;
    if (w < 1024 && N >= (idx_t)w) return 0; /* value cannot fit anyway */
    seq_t p = 1;
    p <<= N;
    seq_t m = (p - 1) / 3;
    seq_t t = m;
    t <<= (N - n - 1);
    return t + m + m + 1;
}
