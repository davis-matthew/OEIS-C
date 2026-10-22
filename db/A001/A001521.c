/*
 * A001521  a(1) = 1; thereafter a(n+1) = floor(sqrt(2*a(n)*(a(n)+1))).
 * https://oeis.org/A001521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = floor(sqrt(2)^(n-1)) + floor(sqrt(2)^(n-2)) for n > 1,
 *            a(1) = 1 (equivalently a(1) = 1, a(n+1) =
 *            floor(sqrt(2*a(n)*(a(n)+1)))). floor(sqrt(2^k)) is 2^(k/2) for
 *            even k; for odd k = 2m+1 it is floor(2^m*sqrt(2)), found one
 *            binary digit at a time from a running remainder, so no
 *            intermediate v...
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   a001521_fsqrt, a001521_pow2, a001521_sqrt2_scaled
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001521_fsqrt.h"
#include "../utils/a001521_pow2.h"
#include "../utils/a001521_sqrt2_scaled.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    return a001521_fsqrt(n - 1) + a001521_fsqrt(n - 2);
}
