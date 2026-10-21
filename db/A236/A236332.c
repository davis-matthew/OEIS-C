/*
 * A236332  The number of orbits of 4-tuples of the dihedral group of order 2n
 *          acting on {1,2,...,n}.
 * https://oeis.org/A236332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9 + 7*(-1)^n + 2*n^3)/4
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the sequence stops at n >= 1 (offset 1). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* not defined for n=0 – safe fallback */

    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;        /* n^2 */
    seq_t nn3 = nn2 * nn;       /* n^3 */
    seq_t term = 2 * nn3;       /* 2*n^3 */

    /* add 16 if n is even (9+7), add 2 if n is odd (9-7) */
    term += (n & 1) ? (seq_t)2 : (seq_t)16;

    return term / 4;           /* integer division, guaranteed exact */
}
