/*
 * A165495  First result not divisible by 4 when iterating k -> k+tau(k) from
 *          2(2n-1)^2.
 * https://oeis.org/A165495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165495
 *
 * Definition: For each positive integer n, begin with
 *   k = 2 * (2n − 1)².
 * Repeatedly apply the map k → k + τ(k), where τ(k) is the number of
 * divisors of k.  After each step check the new value: if it is not
 * congruent to 0 modulo 4 stop and return it.  This always returns the
 * first result not divisible by 4 after at least one iteration.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* guard for unexpected zero index */
    if (n == 0) return 0;

    /* two_n_minus_one = 2*n - 1   (no overflow for idx_t values used) */
    seq_t two_n_minus_one = (seq_t)2 * (seq_t)n - (seq_t)1;

    /* starting value: 2 * (2n-1)² */
    seq_t k = (seq_t)2 * two_n_minus_one * two_n_minus_one;

    /* Perform at least one iteration, then stop when k % 4 != 0 */
    do
    {
        /* divisor_count_u64 expects a uint64_t argument; the current
         * k is guaranteed to fit in 64 bits for the range of n of
         * interest (up to ~10⁴). */
        uint64_t dt = divisor_count_u64((uint64_t)k);
        k += (seq_t)dt;
    } while ((k % (seq_t)4U) == 0U);

    return k;
}
