/*
 * A034673  Sum of n-th powers of divisors of 120.
 * https://oeis.org/A034673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|120} d^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const uint64_t divisors[16] = {
        1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 6ULL,
        8ULL,10ULL,12ULL,15ULL,20ULL,24ULL,
       30ULL,40ULL,60ULL,120ULL
    };
    size_t count = 16;
    seq_t sum = 0;

    for (size_t i = 0; i < count; ++i) {
        seq_t pow = 1;
        uint64_t d = divisors[i];
        for (idx_t k = 0; k < n; ++k) {
            pow *= (seq_t)d;
        }
        sum += pow;
    }
    return sum;
}
