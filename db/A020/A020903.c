/*
 * A020903  Lim f(f(...f(n))) where f is the fractal sequence given by
 *          f(n)=A002260(n+1).
 * https://oeis.org/A020903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = limit of iterating f until 1 or 2, where f(n) is the nth
 *            term of the fractal ladder sequence A002260
 *            (1,2,1,2,3,1,2,3,4,1,…)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   f_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/f_idx.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)n;

    idx_t cur = n;
    while (cur > 2)                                  /* iterate until reaching 1 or 2 */
        cur = f_idx(cur);
    return (seq_t)cur;
}
