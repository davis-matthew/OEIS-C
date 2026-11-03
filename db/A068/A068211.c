/*
 * A068211  Largest prime factor of Euler totient function phi(n).
 * https://oeis.org/A068211
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of φ(n) (n ≥ 3)
 * Range:     u8 n <= 287, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 114 terms through n = 977 (2026-11-01)
 * Helpers:   largest_prime_factor, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Compute φ(n).  For n ≥ 3 we always have φ(n) ≥ 2. */
    seq_t phi_val = totient(n);

    /* Find the largest prime factor of φ(n).  The result fits in 64 bits
       for the range of n we care about, so we safely cast to uint64_t. */
    uint64_t lpf = largest_prime_factor((uint64_t)phi_val);

    return (seq_t)lpf;   /* cast back to the generic output type */
}
