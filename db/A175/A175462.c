/*
 * A175462  Number of divisors of integers of the form 5 + 8n.
 * https://oeis.org/A175462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(5 + 8*n) (number of divisors of 5 + 8*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_ull.h"

seq_t a(idx_t n)
{
    /* Compute 5 + 8*n using 64‑bit arithmetic.  For n within reasonable
       bounds this will not overflow. */
    uint64_t m = 5ull + 8ull * (uint64_t)n;
    return divisor_count_ull(m);
}
