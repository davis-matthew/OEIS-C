/*
 * A145064  Reduced numerators of the first convergent to the cube root of n
 *          using the recursion x = (2*x+n/x^2)/3.
 * https://oeis.org/A145064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(0)=2, a(1)=1, a(2)=4, a(3)=5, a(4)=2, a(5)=7;
 *            a(n)=2*a(n-3)-a(n-6) for n>5
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[6] = { (seq_t)2, (seq_t)1, (seq_t)4,
                                   (seq_t)5, (seq_t)2, (seq_t)7 };

    if (n < 6)
        return init[n];

    seq_t buf[6];
    for (size_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = 2 * buf[(i - 3) % 6] - buf[(i - 6) % 6];
        buf[i % 6] = val;
    }
    return buf[n % 6];
}
