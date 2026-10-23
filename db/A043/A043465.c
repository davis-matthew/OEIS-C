/*
 * A043465  Numbers having one 3 in base 9.
 * https://oeis.org/A043465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑9 representation
 *            contains exactly one digit equal to 3
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_one_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_one_three.h"

seq_t a(idx_t n)
{
    seq_t x = 1;
    idx_t seen = 0;
    for (;;) {
        if (has_one_three(x)) {
            ++seen;
            if (seen == n) return x;
        }
        x++;
    }
}
