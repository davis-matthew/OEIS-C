/*
 * A134007  a(n) = 1^n + 3^n + 5^n + 7^n + 9^n.
 * https://oeis.org/A134007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 3^n + 5^n + 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    sum += pow_uint(1, n);
    sum += pow_uint(3, n);
    sum += pow_uint(5, n);
    sum += pow_uint(7, n);
    sum += pow_uint(9, n);
    return sum;
}
