/*
 * A018346  Divisors of 232.
 * https://oeis.org/A018346
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

/* Divisors of 232 in increasing order */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of the 8 divisors of 232 */
    static bool initialized = false;
    static seq_t divs[8];

    if (!initialized) {
        const uint64_t N = 232ULL;
        uint64_t low[8];
        uint64_t high[8];
        size_t lcnt = 0, hcnt = 0;
        for (uint64_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                low[lcnt++] = i;
                if (i != N / i) {
                    high[hcnt++] = N / i;
                }
            }
        }
        size_t pos = 0;
        for (size_t i = 0; i < lcnt; ++i) {
            divs[pos++] = (seq_t)low[i];
        }
        for (size_t i = 0; i < hcnt; ++i) {
            divs[pos++] = (seq_t)high[hcnt - 1 - i];
        }
        initialized = true;
    }

    if (n == 0 || n > 8) {
        return (seq_t)0;
    }
    return divs[n - 1];
}
