/*
 * A179083  Even numbers having an odd sum of digits in their decimal
 *          representation.
 * https://oeis.org/A179083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Even numbers whose decimal digit sum is odd
   a(n) = { k ∈ 2ℕ | sumdigits(k) mod 2 = 1 }   (n ≥ 1) */
#include <stdbool.h>
#include "../utils/digit_sum.h"

static bool odd_digit_sum(seq_t v) {
    seq_t s = digit_sum(v);        /* sum of the decimal digits */
    long long t = (long long)s;    /* the sum is < 1000 for all practically relevant values */
    return (t & 1LL) != 0;
}

seq_t a(idx_t n) {
    seq_t x = 0;           /* candidate even numbers, starting from 0 */
    idx_t count = 0;       /* number of terms found so far */

    while (1) {
        if (odd_digit_sum(x)) {
            ++count;
            if (count == n) return x;
        }
        x += 2;           /* next even integer */
    }
}
