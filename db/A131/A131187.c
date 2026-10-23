/*
 * A131187  a(n) = the number of positive integers < n that are neither a
 *          divisor of n nor a divisor of (n+1).
 * https://oeis.org/A131187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2 - d(n) - d(n+1)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(n) – number of positive divisors of n */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* number of divisors of n and n+1 */
    seq_t dn  = (seq_t)divisor_count_u64(n);
    seq_t dn1 = (seq_t)divisor_count_u64(n + 1);

    /* compute a(n) using the formula */
    return (seq_t)n + (seq_t)2 - dn - dn1;
}
