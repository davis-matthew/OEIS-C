/*
 * A072056  Number of divisors of 2*prime(n)+1.
 * https://oeis.org/A072056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A072055(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of 2*prime(n)+1 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1-indexed) */
    uint32_t p = get_prime(n);

    /* Compute 2*p + 1 as uint64_t to avoid overflow */
    uint64_t m = ((uint64_t)p) * 2ULL + 1ULL;

    /* Return the divisor count as seq_t */
    return (seq_t)divisor_count_u64(m);
}
