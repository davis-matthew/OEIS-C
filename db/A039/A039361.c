/*
 * A039361  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 5's.
 * https://oeis.org/A039361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Find the n-th integer whose base-9 representation contains the
   same (non‑zero) number of 0's and 5's. */
#include <stdbool.h>
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* not used, offset starts at 1 */
    idx_t found = 0;
    seq_t val = 1;
    while (true) {
        seq_t x = val;
        unsigned int zeros = 0, fives = 0;
        while (x > 0) {
            unsigned int d = (unsigned int)(x % 9);
            if (d == 0) ++zeros;
            if (d == 5) ++fives;
            x /= 9;
        }
        if (zeros > 0 && zeros == fives) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
}
