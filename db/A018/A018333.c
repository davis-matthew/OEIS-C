/*
 * A018333  Divisors of 204.
 * https://oeis.org/A018333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 204 in ascending order (a(1)=1,
 *            a(12)=204)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for divisors of 204 */
    static seq_t cache[205];
    static size_t count = 0;
    if (count == 0) {            /* compute once */
        const idx_t N = 204;
        for (idx_t d = 1; d <= N; ++d) {
            if (((seq_t)N % (seq_t)d) == 0) {
                cache[count++] = (seq_t)d;
            }
        }
    }
    if (n == 0 || (size_t)(n - 1) >= count) {
        return (seq_t)0;
    }
    return cache[n - 1];
}
