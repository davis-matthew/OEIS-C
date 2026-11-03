/*
 * A061822  Multiples of 6 containing only digits 0,...,6.
 * https://oeis.org/A061822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th multiple of 6 whose decimal digits are all ≤6
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether all decimal digits of x are ≤6. */
static bool digits_at_most_six(seq_t x)
{
    if (x == 0) return true;          /* 0 has only one digit 0 */
    while (x > 0) {
        seq_t d = x % (seq_t)10;      /* current last digit */
        if (d > 6) return false;      /* digit too large */
        x /= (seq_t)10;               /* drop the last digit */
    }
    return true;
}

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
