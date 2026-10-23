/*
 * A034669  Sum of n-th powers of divisors of 56.
 * https://oeis.org/A034669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 4^n + 8^n + 7^n + 14^n + 28^n + 56^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    static const uint64_t divs[8] = {1ULL, 2ULL, 4ULL, 8ULL, 7ULL, 14ULL, 28ULL, 56ULL};
    seq_t sum = 0;
    for (int i = 0; i < 8; ++i)
        sum += pow_uint(divs[i], n);
    return sum;
}
