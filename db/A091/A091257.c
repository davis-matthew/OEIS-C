/*
 * A091257  Multiplication table A x B computed for polynomials over GF(2),
 *          where (A,B) runs as (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),...
 * https://oeis.org/A091257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   a(n) = product over GF(2) of the polynomials represented by
 *            integers A and B, where (A,B) is the nth pair in the
 *            anti‑diagonal enumeration: (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),
 * Range:     u8 n <= 480, u16 n <= 9828, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   mulGF2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mulGF2.h"

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        d++;

    /* previous diagonal count */
    idx_t prev = (d - 1) * d / 2;

    /* position within diagonal */
    idx_t i = n - prev;                /* 1 <= i <= d */
    seq_t A = (seq_t)i;
    seq_t B = (seq_t)(d + 1 - i);

    return mulGF2(A, B);
}
