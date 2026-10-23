/*
 * A175928  Number of bases in which the concatenation of the consecutive
 *          increasing integers 1...n is greater than the concatenation of
 *          consecutive decreasing integers n...1 in the same base.
 * https://oeis.org/A175928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of bases b >= 2 such that the base‑b concatenation
 *            of 1,2,…,n is lexicographically greater than the base‑b
 *            concatenation of n,n-1,…,1. The value is obtained by testing
 *            every base b = 2…n and comparing the two concatenations digit by
 *            digit
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   compare_inc_greater, get_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/compare_inc_greater.h"
#include "../utils/get_digits_uint64.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 2; values for n < 2 are irrelevant. */
    if (n < 2) return (seq_t)0;

    seq_t count = 0;
    for (idx_t base = 2; base <= n; ++base)
        if (compare_inc_greater(n, (uint64_t)base))
            count++;

    return count;
}
