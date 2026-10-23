/*
 * A130014  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+881)^2 = y^2.
 * https://oeis.org/A130014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 6*a(n-3) - a(n-6) + 1762  for n > 6
   a(1)=0, a(2)=43, a(3)=2440, a(4)=2643, a(5)=2860, a(6)=16443
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial six terms */
    static const seq_t init[6] = {
        (seq_t)0,
        (seq_t)43,
        (seq_t)2440,
        (seq_t)2643,
        (seq_t)2860,
        (seq_t)16443
    };

    if (n <= 6)
        return init[(size_t)n - 1];

    /* ring buffer holding a(i-6) .. a(i-1) */
    seq_t buf[6];
    for (size_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    seq_t val = 0;
    for (idx_t i = 7; i <= n; ++i)
    {
        val = 6 * buf[3] - buf[0] + 1762;
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }
    return val;
}
