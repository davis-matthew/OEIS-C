/*
 * A034667  Sum of n-th powers of divisors of 40.
 * https://oeis.org/A034667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n+2^n+4^n+5^n+8^n+10^n+20^n+40^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    static const uint64_t divs[8] = {
        1ULL, 2ULL, 4ULL, 5ULL, 8ULL, 10ULL, 20ULL, 40ULL
    };
    seq_t sum = 0;
    for (idx_t i = 0; i < 8; ++i) {
        sum += pow_uint(divs[i], n);
    }
    return sum;
}
