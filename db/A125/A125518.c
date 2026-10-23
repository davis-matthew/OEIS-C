/*
 * A125518  a(n) = tau(n) * prime(n).
 * https://oeis.org/A125518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) * prime(n)
 * Range:     u8 n <= 15, u16 n <= 419, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* tau(n) = number of divisors of n, computed via the utility */
    seq_t tau = divisor_count_u64((uint64_t)n);
    /* prime(n) = the n-th prime (1‑based), computed via the utility */
    seq_t p = get_prime(n);
    return tau * p;
}
