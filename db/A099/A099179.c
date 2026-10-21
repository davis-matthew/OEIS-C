/*
 * A099179  Iterated tetrahedral numbers.
 * https://oeis.org/A099179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Tet( a(n-1) ), a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   tetra
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Tet(x) = x*(x+1)*(x+2)/6  */

#include <stddef.h>
#include "../utils/tetra.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 */
    if (n == 0)
        return 0; /* undefined offset, return 0 */
    seq_t val = 2;               /* a(1) */
    for (idx_t i = 1; i < n; ++i)
        val = tetra(val);
    return val;
}
