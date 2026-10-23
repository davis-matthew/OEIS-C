/*
 * A137002  Numbers k such that k and k^2 use only the digits 1, 2 and 5.
 * https://oeis.org/A137002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137002: Numbers k such that k and k^2 use only the digits 1, 2 and 5.
   The sequence terminates after the four terms 1, 5, 11, 15.  */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Number of terms in the sequence */
    static const seq_t values[] = { (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)15 };
    static const idx_t count = (idx_t)(sizeof values / sizeof *values);

    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    if (n <= count)
        return values[(size_t)(n - 1)];

    return (seq_t)0;              /* sequence terminates – no term for n > count */
}
