/*
 * A105398  A simple "Fractal Jump Sequence" (FJS).
 * https://oeis.org/A105398
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(3-(-1)^n) (period 2: 4,8)
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) {
        /* behaviour for n < 4 is unspecified; return 0 */
        return (seq_t)0;
    }
    /* n is unsigned; odd indices give 8, even indices give 4 */
    return (seq_t)((n & 1) ? 8 : 4);
}
