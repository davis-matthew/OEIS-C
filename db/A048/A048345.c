/*
 * A048345  a(n)^2 is the smallest square containing exactly n 0's.
 * https://oeis.org/A048345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=0; a(2)=10; a(3)=320; a(n)=10*a(n-2) for n>3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    /* Even indices:  a(n) = 10^(n/2) */
    if ((n & 1) == 0) {
        return pow10(n / 2);
    }
    /* Odd indices ≥3 : a(n) = 320 * 10^((n-3)/2) */
    seq_t p = pow10((n - 3) / 2);
    return mul_small(p, 320U);
}
