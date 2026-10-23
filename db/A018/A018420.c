/*
 * A018420  Divisors of 374.
 * https://oeis.org/A018420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 374 in ascending order */
#include <stdbool.h>
seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool init = false;
    if (!init) {
        idx_t count = 0;
        const idx_t N = 374;
        for (idx_t i = 1; i <= N; ++i) {
            if (N % i == 0) {
                divisors[count++] = (seq_t)i;
            }
        }
        init = true;
    }
    if (n == 0 || n > 8) return (seq_t)0;
    return divisors[n - 1];
}
