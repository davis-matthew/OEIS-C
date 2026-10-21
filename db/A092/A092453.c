/*
 * A092453  Numbers that either contain the digit 4 or are divisible by 4.
 * https://oeis.org/A092453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer (0‑indexed) that contains digit 4 or is
 *            divisible by 4
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_digit4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_digit4.h"

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t x    = 0;

    while (true) {
        /* 0 is divisible by 4, so it satisfies the property. */
        if ((x % 4 == 0) || contains_digit4(x)) {
            if (seen == n)
                return x;
            ++seen;
        }
        ++x;
    }
}
