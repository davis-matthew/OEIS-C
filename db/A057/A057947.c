/*
 * A057947  n has ambiguous representations in "bad hexadecimal": numbers with
 *          the digit 1 followed by a digit less than 6.
 * https://oeis.org/A057947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th non‑negative integer whose decimal representation
 *            contains a digit '1' immediately followed by a digit 0–5. The
 *            sequence starts 10, 11, 12, 13, 14, 15, 100, …
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_bad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_bad.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of qualifying values seen */
    seq_t v = (seq_t)10;      /* first candidate (a(0) = 10) */

    while (1)
    {
        if (has_bad(v))
        {
            if (count == n)
                return v;
            ++count;
        }
        ++v;                   /* Proceed to next integer. */
    }
}
