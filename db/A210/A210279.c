/*
 * A210279  a(n) = (6n)!/6^n.
 * https://oeis.org/A210279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6n)!/6^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t limit = n * 6;
    for (idx_t i = 1; i <= limit; ++i)
    {
        result *= (seq_t)i;
        if ((i % 6) == 0)
            result /= (seq_t)6;
    }
    return result;
}
