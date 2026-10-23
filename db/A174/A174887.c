/*
 * A174887  Numbers m such that sum of cubes of their digits > m.
 * https://oeis.org/A174887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   numbers m such that sum of cubes of digits > m
 * Range:     u8 n <= 182, u16 n <= 565, u32 n <= 565, u64 n <= 565,
 *            u128 n <= 565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* precompute all such m (at most 600 terms) */
    static seq_t table[600];
    static size_t count = 0;
    static bool ready = false;

    if (!ready) {
        for (int m = 1; m <= 1999; ++m) {
            unsigned int sum = 0;
            int temp = m;
            while (temp > 0) {
                unsigned int d = temp % 10;
                sum += d * d * d;
                temp /= 10;
            }
            if (sum > (unsigned int)m) {
                table[count++] = (seq_t)m;
            }
        }
        ready = true;
    }

    if (n == 0) return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= count) return 0;
    return table[idx];
}
