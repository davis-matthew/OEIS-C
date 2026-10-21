/*
 * A061473  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 4.
 * https://oeis.org/A061473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   altsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061473: a(n) = the n‑th positive integer whose alternating
   digit sum (first digit + second digit – third digit + …, starting
   with a plus for the leftmost digit) equals 4. */

#include <stddef.h>
#include "../utils/altsum.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;

    while (1) {
        if (altsum(cur) == 4) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
