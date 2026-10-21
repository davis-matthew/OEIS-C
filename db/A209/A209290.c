/*
 * A209290  Number of elements whose preimage is the empty set summed over all
 *          functions f:{1,2,...,n}->{1,2,...,n}.
 * https://oeis.org/A209290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    const seq_t base = (seq_t)(n - 1);
    const seq_t power = pow_int(base, n);
    return (seq_t)n * power;
}
