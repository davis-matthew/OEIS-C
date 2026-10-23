/*
 * A139081  a(n) = (largest prime power dividing n) + (largest prime power
 *          dividing (n+1)).
 * https://oeis.org/A139081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034699(n) + A034699(n+1) where A034699(n) denotes the
 *            largest prime power dividing n (for n ≥ 1)
 * Range:     u8 n <= 177, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_power_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_power_uint64.h"

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;          /* idx_t is uint64_t */
    seq_t first  = largest_prime_power_uint64(u);
    seq_t second = largest_prime_power_uint64(u + 1);
    return first + second;
}
