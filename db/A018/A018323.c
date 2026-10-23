/*
 * A018323  Divisors of 184.
 * https://oeis.org/A018323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 184 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool initialized = false;

    if (!initialized) {
        idx_t cnt = 0;
        for (uint64_t i = 1; i <= 184; ++i) {
            if (184 % i == 0) {
                divisors[cnt++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n == 0) {
        return (seq_t)0;
    }

    idx_t idx = n - 1;
    if (idx >= 8) {
        return (seq_t)0;
    }

    return divisors[idx];
}
