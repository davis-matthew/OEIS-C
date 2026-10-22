/*
 * A003524  Divisors of 2^12 - 1.
 * https://oeis.org/A003524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 4095 = 2^12 - 1 (found by trial
 *            division in increasing order)
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const uint32_t N = 4095u; /* 2^12 - 1 */
    idx_t count = 0;
    uint32_t d;
    for (d = 1; d <= N; d++) {
        if (N % d == 0) {
            count++;
            if (count == n)
                return (seq_t)d;
        }
    }
    return (seq_t)0;
}
