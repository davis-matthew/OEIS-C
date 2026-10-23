/*
 * A187333  a(n) = floor(n/5) + floor(2n/5) + floor(3n/5) + floor(4n/5).
 * https://oeis.org/A187333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/5)+floor(2n/5)+floor(3n/5)+floor(4n/5)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mul_div_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_div_floor.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    total += mul_div_floor(n, 1, 5);
    total += mul_div_floor(n, 2, 5);
    total += mul_div_floor(n, 3, 5);
    total += mul_div_floor(n, 4, 5);
    return total;
}
