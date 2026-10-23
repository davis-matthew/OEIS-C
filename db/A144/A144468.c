/*
 * A144468  Final digit of multiples of 7.
 * https://oeis.org/A144468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 7*n (mod 10)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t a_table[10] = {0,7,4,1,8,5,2,9,6,3};

seq_t a(idx_t n)
{
    return a_table[(size_t)(n % 10)];
}
