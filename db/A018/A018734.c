/*
 * A018734  Divisors of 942.
 * https://oeis.org/A018734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 942, in increasing order (a(1)=1,
 *            a(8)=942)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divisors[100];
    static size_t dcnt = 0;
    static bool init   = false;
    const uint64_t TARGET = 942ULL;

    if (!init) {
        /* enumerate all divisors of 942 and store them in ascending order */
        for (uint64_t d = 1; d <= TARGET; ++d) {
            if (TARGET % d == 0) {
                divisors[dcnt++] = (seq_t)d;
            }
        }
        init = true;
    }

    if (n >= 1 && (idx_t)n <= dcnt) {
        return divisors[n - 1U];
    }
    return 0;
}
