/*
 * A195052  Number of divisors of 24*n - 1 divided by 2.
 * https://oeis.org/A195052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = divisor_count(24*n - 1) / 2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* 24*n - 1 will fit in 64 bits for all n < 2^58 (the practical limits). */
    uint64_t val = 24 * n - 1;
    uint64_t d   = divisor_count_u64(val);
    return (seq_t)(d / 2);
}
