/*
 * A039147  Numbers whose base-10 representation has the same number of 5's
 *          and 7's.
 * https://oeis.org/A039147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-10 representation has equal number of 5's and 7's */
#include <stdbool.h>

static inline bool equal_fives_sevens(seq_t x)
{
    seq_t c5 = 0;
    seq_t c7 = 0;
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 5)
            ++c5;
        else if (d == 7)
            ++c7;
        x = x / 10;
    }
    return c5 == c7;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* invalid index, just return 0 */
        return (seq_t)0;

    idx_t count = 0;     /* number of qualifying terms found */
    seq_t val = 0;       /* current integer to test */

    for (;;) {
        if (equal_fives_sevens(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
