/*
 * A037476  Duplicate of A007093.
 * https://oeis.org/A037476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the n‑th positive integer whose decimal representation
 *            contains no digits 7, 8 or 9 (i.e. the numbers composed only of
 *            digits 0–6). This is equivalent to A007093 with the initial 0
 *            omitted
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool valid_digits(idx_t x)
{
    while (x > 0) {
        if (x % 10 >= 7)   /* a digit 7, 8 or 9 is present */
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1.  If n == 0 return 0 for safety, though not used. */
    if (n == 0) return 0;

    idx_t m   = 0;   /* candidate integer */
    idx_t cnt = 0;   /* how many valid numbers seen so far */

    for (;;) {
        ++m;
        if (valid_digits(m)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)m;
        }
    }
}
