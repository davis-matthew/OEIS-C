/*
 * A018415  Divisors of 366.
 * https://oeis.org/A018415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 366, or 0 if n exceeds the
 *            number of divisors (8)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 366 = 2 · 3 · 61 */
    const uint64_t N = 366;
    if (n == 0) return (seq_t)0;

    idx_t cnt = 0;
    for (uint64_t d = 1; d <= N; ++d) {
        if (N % d == 0) {
            ++cnt;
            if (cnt == n) return (seq_t)d;
        }
    }
    return (seq_t)0;   /* n > number of divisors of 366 */
}
