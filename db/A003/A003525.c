/*
 * A003525  Divisors of 2^14 - 1.
 * https://oeis.org/A003525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^14 - 1 = 16383 (found by
 *            scanning d = 1..16383)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (1ULL << 14) - 1;
    uint64_t d;
    idx_t count = 0;
    for (d = 1; d <= m; d++) {
        if (m % d == 0) {
            count++;
            if (count == n)
                return (seq_t)d;
        }
    }
    return (seq_t)0;
}
