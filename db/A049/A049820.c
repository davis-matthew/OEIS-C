/*
 * A049820  a(n) = n - d(n), where d(n) is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A049820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - d(n) where d(n) is the number of divisors of n
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Sequence offset is 1; behaviour for n < 1 is irrelevant */
    if (n < 1) return 0;

    seq_t nd = (seq_t)n;
    seq_t d  = (seq_t)divisor_count_u64((uint64_t)n);

    return nd - d;
}
