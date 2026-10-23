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
 * Helpers:   has_two_nonzero_squares, is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are the sum of two nonzero squares and not divisible by 17.
   a(1) = 2, a(2) = 5, a(3) = 8, …  (OEIS A100829) */
#include <stdbool.h>
#include "../utils/has_two_nonzero_squares.h"
#include "../utils/is_square.h"

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
