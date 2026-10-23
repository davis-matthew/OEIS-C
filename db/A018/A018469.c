/*
 * A018469  Divisors of 464.
 * https://oeis.org/A018469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 464 (1 ≤ n ≤ 10)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t divisors[10];
    static int initialized = 0;
    if (!initialized) {
        const uint64_t N = 464ULL;
        idx_t count = 0;
        for (uint64_t d = 1; d <= N; ++d) {
            if (N % d == 0) {
                divisors[count++] = (seq_t)d;
            }
        }
        /* divisors now contains the 10 divisors of 464 in ascending order */
        initialized = 1;
    }
    if (n == 0 || n > 10) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
