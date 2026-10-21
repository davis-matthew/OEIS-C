/*
 * A073788  Numbers in base -7.
 * https://oeis.org/A073788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the decimal value obtained by writing n in base –7 and
 *            interpreting the resulting digit string as a base‑10 integer
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    unsigned digits[64];
    unsigned len = 0;

    /* use a signed 128‑bit integer to handle negative values during
       conversion from base –7 to decimal.  Most compilers that understand
       C11/C++17 support the GNU/Clang extension __int128. */
    __int128 t = n;
    while (t != 0) {
        __int128 r = t % 7;    /* remainder may be negative */
        if (r < 0) r += 7;     /* force it into 0..6 */
        digits[len++] = (unsigned)r;
        t = (t - r) / (-7);
    }

    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
