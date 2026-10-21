/*
 * A072677  a(n) = prime(prime(n)+1) where prime(k) is the k-th prime.
 * https://oeis.org/A072677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(prime(n)+1)
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Compute the n-th prime (p_n). */
    uint32_t inner = get_prime(n);
    /* Compute the (inner+1)-st prime, which is a(n). */
    idx_t   outer_idx = (idx_t)inner + 1;
    uint32_t outer = get_prime(outer_idx);
    return (seq_t)outer;
}
