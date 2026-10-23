/*
 * A024050  a(n) = 5^n - n.
 * https://oeis.org/A024050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 5;
    idx_t e = n;

    while (e) {
        if (e & 1) {
            result *= base;
        }
        if (e > 1) {          /* avoid an unnecessary square when e==1 */
            base *= base;
        }
        e >>= 1;
    }

    result -= (seq_t)n;
    return result;
}
