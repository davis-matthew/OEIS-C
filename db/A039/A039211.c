/*
 * A039211  Numbers whose base-12 representation has the same number of 0's
 *          and 2's.
 * https://oeis.org/A039211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑12 representation
 *            contains the same number of 0's and 2's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

bool equal_zero_two(seq_t x) {
    idx_t c0 = 0, c2 = 0;
    while (x != 0) {
        seq_t digit = x % 12;
        if (digit == 0) c0++;
        else if (digit == 2) c2++;
        x = x / 12;
    }
    return c0 == c2;
}

seq_t a(idx_t n) {
    seq_t cur = 1;
    idx_t found = 0;
    while (found < n) {
        if (equal_zero_two(cur)) {
            found++;
            if (found == n)
                return cur;
        }
        cur = cur + 1;
    }
    /* should never reach here */
    return 0;
}
