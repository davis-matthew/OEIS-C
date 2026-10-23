/*
 * A018463  Divisors of 452.
 * https://oeis.org/A018463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 452: a(n) is the nth smallest divisor of 452 */
#include <stdint.h>

seq_t a(idx_t n) {
    static int initialized = 0;
    static seq_t divisors[6];

    if (!initialized) {
        /* enumerate divisors of 452 */
        const uint64_t M = 452;
        idx_t cnt = 0;
        for (uint64_t d = 1; d * d <= M; ++d) {
            if (M % d == 0) {
                divisors[cnt++] = (seq_t)d;
                if (d * d != M)
                    divisors[cnt++] = (seq_t)(M / d);
            }
        }
        /* simple bubble sort to order them ascending */
        for (idx_t i = 0; i < cnt; ++i)
            for (idx_t j = i + 1; j < cnt; ++j)
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        initialized = 1;
    }

    if (n == 0 || n > 6)
        return (seq_t)0;
    return divisors[n - 1];
}
