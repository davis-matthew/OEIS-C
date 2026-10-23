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
 * Helpers:   equal_fives_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-10 representation has equal number of 5's and 7's */
#include <stdbool.h>
#include "../utils/equal_fives_sevens.h"

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
