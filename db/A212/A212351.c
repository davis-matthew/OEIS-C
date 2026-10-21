/*
 * A212351  Maximal number of "good" manifolds in an n-nice polytope.
 * https://oeis.org/A212351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n for n >= 7; a(1)=6, a(2)=8, a(3)=12, a(4)=24,
 *            a(5)=40, a(6)=80 for n < 7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 6) {
        static const seq_t small[6] = {(seq_t)6, (seq_t)8, (seq_t)12, (seq_t)24, (seq_t)40, (seq_t)80};
        return small[n - 1];
    } else {
        return pow2((idx_t)n);
    }
}
