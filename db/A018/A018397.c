/*
 * A018397  Divisors of 332.
 * https://oeis.org/A018397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 332, listed in increasing order
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t divisors[64];
    static size_t cnt = 0;
    static int initialized = 0;

    if (!initialized) {
        for (uint64_t i = 1; i <= 332; ++i) {
            if (332 % i == 0) {
                divisors[cnt++] = (seq_t)i;
            }
        }
        initialized = 1;
    }

    if (n == 0 || (idx_t)n > cnt) {
        return (seq_t)0;
    }
    return divisors[(size_t)(n - 1)];
}
