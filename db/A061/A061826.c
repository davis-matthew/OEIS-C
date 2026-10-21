/*
 * A061826  Multiples of 7 containing only even digits.
 * https://oeis.org/A061826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   all_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth multiple of 7 whose decimal digits are all even. */

#include <stdbool.h>
#include "../utils/all_even_digits.h"

/* Return the n-th term of the sequence A061826 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many qualifying numbers have been seen */
    seq_t value = 0;        /* current multiple of 7 to test */

    for (;;) {
        if (all_even_digits(value)) {
            ++count;
            if (count == n)
                return value;
        }
        value += (seq_t)7;   /* advance to the next multiple of 7 */
    }
}
