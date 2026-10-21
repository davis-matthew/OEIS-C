/*
 * A062907  Numbers k such that k and its reversal are both multiples of 19.
 * https://oeis.org/A062907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_mult19
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062907:
 * Numbers k such that k and its decimal reversal are both multiples of 19.
 * a(n) is the nth smallest such number, starting with a(1)=0. */
#include <stdbool.h>
#include "../utils/rev_mult19.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 0;; k += (seq_t)19) {
        if (rev_mult19(k)) {
            ++count;
            if (count == n)
                return k;
        }
    }
}
