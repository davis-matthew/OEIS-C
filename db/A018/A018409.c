/*
 * A018409  Divisors of 354.
 * https://oeis.org/A018409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) returns the nth divisor of 354 in ascending order. For n >
 *            8 the sequence is undefined; the function returns 0
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[8];
    static size_t dcount = 0;

    if (!initialized) {
        const uint64_t N = 354;          /* the number whose divisors are listed */
        size_t i = 0;
        for (uint64_t x = 1; x <= N; ++x) {
            if (N % x == 0) {
                divisors[i++] = (seq_t)x;
            }
        }
        dcount = i;
        initialized = true;
    }

    if (n == 0) {
        return 0;                      /* safety: offset is 1 */
    }

    idx_t idx = n - 1;
    if ((size_t)idx < dcount) {
        return divisors[(size_t)idx];
    } else {
        return 0;                      /* beyond defined terms */
    }
}
