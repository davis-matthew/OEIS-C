/*
 * A018488  Divisors of 498.
 * https://oeis.org/A018488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 498 in increasing order (1-indexed)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the list of divisors of 498 only once */
    static seq_t divisors[8];
    static size_t count = 0;

    if (count == 0) {
        const uint64_t N = 498;
        size_t idx = 0;
        for (uint64_t i = 1; i <= N; ++i) {
            if (N % i == 0) {
                divisors[idx++] = (seq_t)i;
            }
        }
        count = idx; /* should be 8 for 498 */
    }

    if (n >= 1 && (size_t)n <= count) {
        return divisors[n - 1];
    }
    return (seq_t)0; /* undefined for n > number of divisors */
}
