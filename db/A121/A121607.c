/*
 * A121607  a(n) = (n^3 + n)*3^n.
 * https://oeis.org/A121607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n)*3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, behaviour for 0 is irrelevant */
    seq_t nn = (seq_t)n;              /* convert index to seq_t to avoid overflow in intermediate steps */
    seq_t t  = nn * (nn * nn + 1);    /* compute n^3 + n = n*(n^2 + 1) */
    t = t * pow3(n);                  /* multiply by 3^n */
    return t;
}
