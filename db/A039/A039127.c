/*
 * A039127  Numbers whose base-10 representation has the same number of 1's
 *          and 9's.
 * https://oeis.org/A039127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth non‑negative integer whose decimal
   representation contains an equal number of digits 1 and 9.
   We generate numbers in increasing order, test each one, and
   return the n‑th that satisfies freq[1] == freq[9]. */

#include <stdbool.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 has no 1’s and no 9’s */
    if (n == 1)
        return (seq_t)0;

    idx_t count = 1;            /* we already counted 0 */
    seq_t val = 1;              /* start checking from 1 */
    unsigned int freq[10];

    for (;; ++val)
    {
        digit_frequencies(val, freq);
        if (freq[1] == freq[9]) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
