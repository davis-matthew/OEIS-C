/*
 * A062821  Number of divisors of totient of n.
 * https://oeis.org/A062821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(phi(n)) (number of divisors of Euler’s totient)
 * Range:     u8 n <= 293, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   divisor_count_u64_local, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/divisor_count_u64_local.h"

seq_t a(idx_t n)
{
    seq_t phi_val = phi(n);           /* phi(n) fits in seq_t for all tests */
    uint64_t phi64 = (uint64_t)phi_val; /* cast to 64‑bit for small‑divisor routine */
    return divisor_count_u64_local(phi64);
}
