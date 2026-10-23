/*
 * A047163  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= n/2.
 * https://oeis.org/A047163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} binomial(floor(n/2), k) * binomial(ceil(n/2),
 *            3*k)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 80, u128 n <= 156,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* safety, offset starts at 1 */
    idx_t low  = n / 2;                  /* floor(n/2) */
    idx_t high = n - low;                /* ceil(n/2) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= low; ++k) {
        idx_t high_needed = 3 * k;
        if (high_needed > high) break;  /* no more valid k */
        seq_t ways_low  = binomial(low,  k);
        seq_t ways_high = binomial(high, high_needed);
        sum += ways_low * ways_high;
    }
    return sum;
}
