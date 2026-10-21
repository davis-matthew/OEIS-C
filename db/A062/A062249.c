/*
 * A062249  a(n) = n + d(n), where d(n) = number of divisors of n, cf.
 *          A000005.
 * https://oeis.org/A062249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + d(n)
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(n) = number of positive divisors of n  */

#include "../utils/_divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type and add the divisor count.
       The helper _divisor_count_u64 works on uint64_t and returns the
       number of divisors of n.  Casting to seq_t ensures the result is
       computed in the correct integer width for this build.  */
    return (seq_t)n + (seq_t)_divisor_count_u64(n);
}
