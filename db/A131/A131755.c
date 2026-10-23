/*
 * A131755  a(n) = floor of the average of distances between consecutive
 *          positive divisors of n. Also, a(n) = floor((n-1)/(d(n)-1)), where
 *          d(n) = A000005(n).
 * https://oeis.org/A131755
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)/(d(n)-1)) where d(n) = A000005(n), the number
 *            of positive divisors of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 in that case. */
    if (n < 2)
        return 0;

    /* d(n) – number of positive divisors of n. */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);

    /* Numerator: n-1 (cast to seq_t first, as required). */
    seq_t num = (seq_t)n - 1;

    /* Denominator: d-1 (d>=2 for n>=2). */
    seq_t denom = d - 1;

    return num / denom;   /* integer division gives the floor. */
}
