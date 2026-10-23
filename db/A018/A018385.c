/*
 * A018385  Divisors of 308.
 * https://oeis.org/A018385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 308 in increasing order
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divis[12];
    static size_t count = 0;

    if (count == 0) {
        /* Build the divisor list of 308 */
        const seq_t N = (seq_t)308;
        size_t idx = 0;
        for (seq_t d = (seq_t)1; d <= N; ++d) {
            if (N % d == 0) {
                divis[idx++] = d;
            }
        }
        count = idx;
    }

    if (n == (idx_t)0 || n > (idx_t)count) {
        return (seq_t)0;
    }
    return divis[(size_t)(n - (idx_t)1)];
}
