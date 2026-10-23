/*
 * A130674  a(n) = d(n)!, where d denotes the number of divisors of n.
 * https://oeis.org/A130674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n)! where d(n) is the number of divisors of n
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    /* divisor_count_u64 returns uint64_t, which fits in idx_t for the range tested */
    uint64_t d = divisor_count_u64((uint64_t)n);
    return fact_upto((idx_t)d);
}
