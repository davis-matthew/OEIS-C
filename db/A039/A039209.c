/*
 * A039209  Numbers whose base-11 representation has the same number of 9's
 *          and 10's.
 * https://oeis.org/A039209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_9_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-11 representation has the same number of 9's and 10's.
   The function returns the n-th value of that sequence (1‑based). */

#include <stdbool.h>
#include "../utils/has_equal_9_and_10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; undefined below but harmless */
    seq_t v = 0;
    idx_t count = 0;
    while (count < n) {
        if (has_equal_9_and_10(v)) ++count;
        ++v;
    }
    return v - 1;                  /* v was incremented after last match */
}
