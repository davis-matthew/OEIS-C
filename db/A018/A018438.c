/*
 * A018438  Divisors of 408.
 * https://oeis.org/A018438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 408 (sorted in ascending order)
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divisors[408];
    static size_t divisor_count = 0;
    static bool initialized = false;

    if (!initialized) {
        const uint64_t M = (uint64_t)408;
        for (uint64_t i = 1; i <= M; ++i) {
            if (M % i == 0) {
                divisors[divisor_count++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;
    if ((size_t)(n - 1) < divisor_count)
        return divisors[n - 1];
    return (seq_t)0;
}
