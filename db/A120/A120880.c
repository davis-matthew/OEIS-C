/*
 * A120880  G.f. satisfies: A(x) = A(x^3)*(1 + 2*x + x^2); thus a(n) =
 *          2^A062756(n), where A062756(n) is the number of 1's in the ternary
 *          expansion of n.
 * https://oeis.org/A120880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^A062756(n), where A062756(n) = number of 1's in the
 *            ternary expansion of n
 * Range:     u8 n <= 3279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp % 3 == 1) ++cnt;
        tmp /= 3;
    }
    return pow2(cnt);
}
