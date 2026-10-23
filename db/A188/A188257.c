/*
 * A188257  [nr+kr]-[nr]-[kr], where r=sqrt(5), k=2, [ ]=floor.
 * https://oeis.org/A188257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(n+2)√5⌋ − ⌊n√5⌋ − 4 Using the identity ⌊m√5⌋ = ⌊√(5
 *            m²)⌋, which avoids floating point arithmetic
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute (n+2) and n squares */
    seq_t ns  = (seq_t)n;         /* n */
    seq_t ns2 = (seq_t)(n + 2);   /* n+2 */

    /* Compute floor(sqrt(5*(n+2)^2)) and floor(sqrt(5*n^2)) */
    seq_t val1 = floor_sqrt((seq_t)5 * (ns2 * ns2));   /* ⌊(n+2)√5⌋ */
    seq_t val2 = floor_sqrt((seq_t)5 * (ns  * ns ));   /* ⌊n√5⌋   */

    /* Return the result: floor(sqrt(5*(n+2)^2)) - floor(sqrt(5*n^2)) - 4 */
    return val1 - val2 - (seq_t)4;
}
