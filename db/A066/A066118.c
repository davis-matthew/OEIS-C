/*
 * A066118  a(n) = n!*(3*n-1)/2.
 * https://oeis.org/A066118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(3*n-1)/2 (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, but safe guard */
    seq_t f = factorial(n);              /* compute n! */
    seq_t factor = (seq_t)3 * (seq_t)n - 1; /* compute (3*n-1) */
    f *= factor;                         /* multiply */
    f /= 2;                               /* divide by 2 (exact) */
    return f;
}
