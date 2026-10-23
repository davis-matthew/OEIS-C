/*
 * A127422  Duplicate of A001703.
 * https://oeis.org/A127422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = concatenate the integers n, n+1, n+2 (no leading zeros)
 * Range:     i8 n <= 1, i16 n <= 8, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = n * 10^(d(n+1)+d(n+2)) + (n+1) * 10^(d(n+2)) + (n+2) */
/* where d(x) is the number of decimal digits of x (d(0)=1). */

#include <stdint.h>
#include "../utils/pow10.h"

static inline idx_t digit_count_seq(seq_t x)
{
    /* Count digits of a non‑negative integer, treating 0 as 1 digit. */
    if (x == 0)
        return 1;
    idx_t d = 0;
    while (x > 0)
    {
        x /= 10;
        d++;
    }
    return d;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = 0; k < 3; ++k)
    {
        seq_t val = (seq_t)(n + k);
        idx_t d   = digit_count_seq(val);
        result = result * pow10(d) + val;
    }

    return result;
}
