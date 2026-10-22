/*
 * A000066  Smallest number of vertices in trivalent graph with girth
 *          (shortest cycle) = n.
 * https://oeis.org/A000066
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, hard, more, nice
 * Formula:   a(n) = M(n) + e(n), where M(n) = A027383(n-1) is the Moore bound
 *            (M(2k) = 2^(k+1) - 2, M(2k+1) = 3*2^k - 2). The excess e(n) is 0
 *            for n = 3, 4, 5, 6, 8, 12 (generalized polygons / Moore graphs).
 *            For n = 7, 9, 10, 11 the excess is 2, 12, 8, 18; these values
 *            come from published exhaustive computer...
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   moore_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/moore_bound.h"

/* Excess of the (3,g)-cage order over the Moore bound, for g = 3..12. */
static const int cage_excess[10] = {
    0,  /* g=3  : K4                  */
    0,  /* g=4  : K3,3                */
    0,  /* g=5  : Petersen            */
    0,  /* g=6  : Heawood             */
    2,  /* g=7  : McGee               */
    0,  /* g=8  : Tutte-Coxeter       */
    12, /* g=9  : 18 cages on 58      */
    8,  /* g=10 : 3 cages on 70       */
    18, /* g=11 : Balaban 11-cage     */
    0   /* g=12 : generalized hexagon */
};

seq_t a(idx_t n)
{
    seq_t m;
    if (n < 3)
        return 0;
    m = moore_bound(n);
    if (n <= 12)
        m += (seq_t)cage_excess[n - 3];
    return m;
}
