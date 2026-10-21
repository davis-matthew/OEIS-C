/*
 * A061822  Multiples of 6 containing only digits 0,...,6.
 * https://oeis.org/A061822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th multiple of 6 whose decimal digits are all ≤6
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digits_at_most_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits_at_most_six.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                   /* how many valid numbers seen so far */
    for (idx_t k = 0; ; ++k) {         /* iterate over multiples of 6 */
        seq_t val = (seq_t)6 * (seq_t)k;
        if (digits_at_most_six(val)) {
            if (count == n) return val;
            ++count;
        }
    }
}
