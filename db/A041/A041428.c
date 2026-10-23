/*
 * A041428  Numerators of continued fraction convergents to sqrt(230).
 * https://oeis.org/A041428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 182*a(n-2) - a(n-4), a(0)=15, a(1)=91, a(2)=2745,
 *            a(3)=16561
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 15;
    if (n == 1) return 91;
    if (n == 2) return 2745;
    if (n == 3) return 16561;

    seq_t v0 = 15, v1 = 91, v2 = 2745, v3 = 16561, va;
    for (idx_t i = 4; i <= n; ++i) {
        va = 182 * v2 - v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = va;
    }
    return v3;
}
