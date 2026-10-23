/*
 * A043693  Numbers whose base-4 representation has an even number of runs.
 * https://oeis.org/A043693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base-4 representation has
 *            an even number of runs
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset 1; undefined for n=0, but return 0 to avoid UB */
        return (seq_t)0;
    }

    idx_t count = 0;
    for (idx_t num = 1; ; ++num) {
        idx_t x = num;
        int  prev   = -1;
        int  runs   = 0;
        while (x > 0) {
            int digit = x % 4;
            if (digit != prev) {
                ++runs;
                prev = digit;
            }
            x /= 4;
        }
        if ((runs & 1) == 0) {           /* even number of runs */
            ++count;
            if (count == n) {
                return (seq_t)num;
            }
        }
    }
}
