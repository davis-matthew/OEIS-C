/*
 * A016101  (n! - n)/2 for even n.
 * https://oeis.org/A016101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! / 2 - n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factorial.h"


seq_t a(idx_t n)
{
    /* compute (2n)! */
    idx_t m = 2 * n;
    seq_t f = factorial(m);

    /* compute (2n)! / 2 */
    f = f / (seq_t)2;

    /* subtract n */
    f = f - (seq_t)n;

    return f;
}
