/*
 * A174990  Partial sums of A005606.
 * https://oeis.org/A174990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = Σ_{i=0..n} A005606(i) where A005606(i) is the position
 *            (1‑26) of the first letter of the English word for i, treating 0
 *            as “zero” (z → 26)
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   first_letter_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/first_letter_value.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
        sum += first_letter_value(i);
    return sum;
}
