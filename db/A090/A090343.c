/*
 * A090343  Difference between the sums of the prime factors, including
 *          multiplicity, of n and those of n + 4.
 * https://oeis.org/A090343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001414(n) - A001414(n+4) (sum of prime factors, with
 *            multiplicity)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   prime_factor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_factor_sum.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1; return 0 instead */

    uint64_t v1 = (uint64_t)n;
    uint64_t v2 = v1 + 4;
    return prime_factor_sum(v1) - prime_factor_sum(v2);
}
