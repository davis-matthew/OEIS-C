/*
 * A052694  Expansion of e.g.f. (1 + x^3 - 2*x^4)/(1-2*x).
 * https://oeis.org/A052694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * n! + 6 * [n==3] (where [P] is 1 if P else 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)2 * (seq_t)i;   /* multiply by 2*i each step */
    }
    if (n == 3) res += 6;                /* special correction for n = 3 */
    return res;
}
