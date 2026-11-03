/*
 * A100829  Numbers that are a sum of two nonzero squares and not divisible by
 *          17.
 * https://oeis.org/A100829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are the sum of two nonzero squares and not divisible by 17.
   a(1) = 2, a(2) = 5, a(3) = 8, …  (OEIS A100829) */
#include <stdbool.h>
#include "../utils/is_square.h"

/* Check whether x can be written as a^2 + b^2 with a,b > 0. */
static bool has_two_nonzero_squares(seq_t x)
{
    if (x < (seq_t)2)          /* smallest is 1^2 + 1^2 = 2 */
        return false;

    /* Find sqrt(x) as an upper bound for a. */
    seq_t low = (seq_t)0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = (low + high + (seq_t)1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - (seq_t)1;
    }
    seq_t limit = low;

    for (seq_t a = (seq_t)1; a <= limit; ++a) {
        seq_t a2 = a * a;
        if (a2 >= x)
            break;
        seq_t b2 = x - a2;
        if (b2 == (seq_t)0)       /* b would be zero */
            continue;
        if (is_square(b2))
            return true;
    }
    return false;
}

/* Return the n‑th term (1‑indexed) of the sequence. */
seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    seq_t m      = (seq_t)1;

    while (true) {
        if ((m % (seq_t)17) != (seq_t)0) {
            if (has_two_nonzero_squares(m)) {
                ++count;
                if (count == (seq_t)n)
                    return m;
            }
        }
        ++m;
    }
}
