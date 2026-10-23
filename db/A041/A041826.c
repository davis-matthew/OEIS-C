/*
 * A041826  Numerators of continued fraction convergents to sqrt(434).
 * https://oeis.org/A041826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 250*a(n-4) - a(n-8); a(0)=20, a(1)=21, a(2)=104,
 *            a(3)=125, a(4)=5104, a(5)=5229, a(6)=26020, a(7)=31249
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] =
    {
        (seq_t)20, (seq_t)21, (seq_t)104, (seq_t)125,
        (seq_t)5104, (seq_t)5229, (seq_t)26020, (seq_t)31249
    };

    if (n < 8)
        return init[n];

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t term4 = buf[(i - 4) % 8];
        seq_t term8 = buf[(i - 8) % 8];
        buf[i % 8] = (seq_t)250 * term4 - term8;
    }

    return buf[n % 8];
}
