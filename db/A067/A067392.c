/*
 * A067392  Sum of numbers <= n which have common prime factors with n.
 * https://oeis.org/A067392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - n*phi(n)/2 for n >= 2, a(1) = 0. phi(n) is
 *            Euler's totient function
 * Range:     u8 n <= 29, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t nu = (uint64_t)n;
    /* (n * (n + 1)) / 2 */
    seq_t sum = (seq_t)nu * ((seq_t)nu + 1) / 2;
    /* (n * phi(n)) / 2 */
    seq_t sub = (seq_t)nu * (seq_t)phi_uint64(nu) / 2;
    return sum - sub;
}
