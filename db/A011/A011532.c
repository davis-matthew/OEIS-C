/*
 * A011532  Numbers that contain a 2.
 * https://oeis.org/A011532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose decimal representation
 *            contains the digit 2 (search)
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        idx_t x = candidate;
        bool has2 = false;

        while (x > 0) {
            if (x % 10 == 2) {
                has2 = true;
                break;
            }
            x /= 10;
        }

        if (has2) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* This point is never reached for valid n >= 1 */
    return (seq_t)0;
}
