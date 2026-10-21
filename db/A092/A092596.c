/*
 * A092596  Natural numbers n for which sum of decimal digits is greater than
 *          n/2.
 * https://oeis.org/A092596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A092596: Natural numbers n for which the sum of decimal digits is greater than n/2.
   The sequence is finite: 1,2,3,4,5,6,7,8,9,19. 0 is returned for indices beyond its length. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (seq_t m = 1; m <= 19; ++m) {
        seq_t sum = 0;
        seq_t x = m;
        while (x != 0) {
            sum += x % 10;
            x /= 10;
        }
        if (sum > m / 2) {
            ++cnt;
            if (cnt == n) {
                return m;
            }
        }
    }
    return 0;
}
