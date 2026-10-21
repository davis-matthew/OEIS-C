/*
 * A076690  Nearest integer to average of the smallest and largest prime
 *          factors of n (0.5 is rounded to 1).
 * https://oeis.org/A076690
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nearest integer to (p_min(n)+p_max(n))/2, where p_min and
 *            p_max are the smallest and largest prime factors of n; fractions
 *            .5 are rounded up
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_factors.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, but values for n < 2 are irrelevant. */
    if (n < 2)
        return 0;

    uint64_t minp, maxp;
    prime_factors(n, &minp, &maxp);

    /* Compute nearest integer to average, with .5 rounded up. */
    uint64_t sum = minp + maxp;
    uint64_t avg = (sum + 1) / 2;   /* integer division truncates */

    return (seq_t)avg;
}
