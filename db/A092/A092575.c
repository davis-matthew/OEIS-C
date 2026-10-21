/*
 * A092575  Number of representations of n of the form x^2 + 3y^2 with
 *          (x,y)=1.
 * https://oeis.org/A092575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of coprime representations of n as * n = x^2 +
 *            3y^2 with x>0, y>0, gcd(x,y)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_lim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_lim.h"

/* Euclidean algorithm for seq_t values */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0 per OEIS definition */

    int counter = 0;
    for (idx_t x = 1; x * x <= n; ++x) {
        idx_t t = n - x * x;
        if (t % 3 != 0)
            continue;
        idx_t y2 = t / 3;
        if (y2 == 0)          /* y must be >0 */
            continue;
        idx_t y = isqrt_lim(y2);
        if (y * y != y2)      /* not a perfect square */
            continue;
        if (seq_gcd((seq_t)x, (seq_t)y) == 1)
            ++counter;
    }
    return (seq_t)counter;
}
