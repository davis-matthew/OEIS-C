/*
 * A063123  Number of solutions (r,s), 0< r< s, to the equation 1/n = 1/r +
 *          1/s + 1/(r*s).
 * https://oeis.org/A063123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)*tau(n+1)/2
 * Range:     u8 n <= 5983, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(m) is the number of positive divisors of m */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* compute tau(n) and tau(n+1) */
    uint64_t tau_n  = divisor_count_u64((uint64_t)n);
    uint64_t tau_np = divisor_count_u64((uint64_t)(n + 1));

    /* product divided by 2 */
    unsigned __int128 prod = (unsigned __int128)tau_n * tau_np;
    prod /= 2;

    return (seq_t)prod;
}
