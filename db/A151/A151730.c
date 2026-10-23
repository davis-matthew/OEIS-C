/*
 * A151730  Divisors of 9213.
 * https://oeis.org/A151730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* a(n) = n-th smallest divisor of 9213 in increasing order */
seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static size_t divisor_count = 0;

    if (divisor_count == 0) {
        const uint64_t num = 9213ull;
        seq_t small[64];
        seq_t large[64];
        size_t sc = 0, lc = 0;

        for (uint64_t d = 1; d * d <= num; ++d) {
            if (num % d == 0) {
                small[sc++] = d;
                if (d != num / d)
                    large[lc++] = num / d;
            }
        }

        size_t i, pos;
        for (i = 0, pos = 0; i < sc; ++i, ++pos)
            divisors[pos] = small[i];
        for (i = 0; i < lc; ++i, ++pos)
            divisors[pos] = large[lc - i - 1];

        divisor_count = sc + lc;
    }

    if (n == 0)
        return 0;
    if (n <= divisor_count)
        return divisors[n - 1];
    return 0;
}
