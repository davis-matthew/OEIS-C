/*
 * A139149  a(n) = (n!+2)/2.
 * https://oeis.org/A139149
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n! + 2) / 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(2) = 2, a(3) = 4, a(4) = 13, ... */
/* Works for any seq_t and idx_t provided by the build. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.  For n < 2 the
       result is undefined; we return 0 as a harmless placeholder. */
    if (n < 2) {
        return 0;
    }

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return (fact + 2) / 2;
}
