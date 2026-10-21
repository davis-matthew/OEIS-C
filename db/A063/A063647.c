/*
 * A063647  Number of ways to write 1/n as a difference of exactly 2 unit
 *          fractions.
 * https://oeis.org/A063647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (tau(n²) - 1) / 2 where tau(m) = number of divisors of m
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This equals the number of ways to write 1/n as 1/b – 1/c with b≠c.) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_square.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0; return 0 for safety */
        return (seq_t)0;

    seq_t tau = divisor_count_square((uint64_t)n);
    return (tau - (seq_t)1) / (seq_t)2;
}
