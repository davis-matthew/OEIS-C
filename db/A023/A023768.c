/*
 * A023768  Nialpdromes: digits in base 13 are in nonincreasing order.
 * https://oeis.org/A023768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_nialpdrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth nonnegative integer whose base‑13 digits are in
 * nonincreasing order (Nialpdromes).  The function searches
 * sequentially for the n‑th such number. */
#include <stdbool.h>
#include "../utils/is_nialpdrome.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    while (1) {
        if (is_nialpdrome(x)) {
            count++;
            if (count == n) return x;
        }
        x++;
    }
}
