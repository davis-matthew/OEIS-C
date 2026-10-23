/*
 * A034664  Sum of n-th powers of divisors of 24.
 * https://oeis.org/A034664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   pow_int_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A034664: Sum of n-th powers of divisors of 24.
   a(n) = 1^n + 2^n + 3^n + 4^n + 6^n + 8^n + 12^n + 24^n */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_int_small.h"

seq_t a(idx_t n)
{
    /* Divisors of 24 */
    static const uint64_t divs[8] = {1ULL, 2ULL, 3ULL, 4ULL, 6ULL, 8ULL, 12ULL, 24ULL};
    size_t cnt = sizeof(divs) / sizeof(divs[0]);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum = sum + pow_int_small(divs[i], n);
    return sum;
}
