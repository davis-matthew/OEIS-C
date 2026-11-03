/*
 * A060710  Number of subgroups of dihedral group with 2n elements, counting
 *          conjugate subgroups only once, i.e., conjugacy classes of
 *          subgroups of the dihedral group.
 * https://oeis.org/A060710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*tau(n) + (n even ? tau(n/2) : 0) / where tau(k) is the
 *            number of positive divisors of k
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 980 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* tau(n) */
    seq_t tau_n = num_divisors(n);
    /* double it */
    seq_t result = tau_n + tau_n;
    /* if n is even add tau(n/2) */
    if ((n & 1u) == 0u)
        result += num_divisors(n / 2);
    return result;
}
