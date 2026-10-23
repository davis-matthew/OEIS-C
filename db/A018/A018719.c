/*
 * A018719  Divisors of 915.
 * https://oeis.org/A018719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 915 in ascending order (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 915 = 3 * 5 * 61 → 8 divisors */
    static const idx_t N = 915;
    static seq_t divisors[8];
    static idx_t div_cnt = 0;
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 1; i <= N; ++i) {
            if (N % i == 0) {
                divisors[div_cnt++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n < 1 || n > div_cnt) {
        return (seq_t)0;          /* undefined indices */
    }
    return divisors[n - 1];
}
