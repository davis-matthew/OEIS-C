/*
 * A072451  Number of odd terms in the reduced residue system of 2*n-1.
 * https://oeis.org/A072451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(2*n-1)/2 (a(1)=1)
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_uint64_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_uint64_generic.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1 */
    if (n == 1) return 1;

    /* Compute m = 2*n-1 (odd).  n is guaranteed to be at least 2. */
    uint64_t m = (uint64_t)2 * (uint64_t)n - 1ULL;
    uint64_t phi = (uint64_t)phi_uint64_generic(m);
    return (seq_t)(phi / 2);
}
