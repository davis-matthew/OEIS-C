/*
 * A099774  Number of divisors of 2*n-1.
 * https://oeis.org/A099774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of 2*n-1 (τ(2*n-1))
 * Range:     u8 n <= 2189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_odd.h"

seq_t a(idx_t n)
{
    /* 2*n-1 is guaranteed odd */
    uint64_t m = (uint64_t)(2 * (seq_t)n - 1);
    return divisor_count_odd(m);
}
