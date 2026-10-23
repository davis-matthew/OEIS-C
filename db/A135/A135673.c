/*
 * A135673  Ceiling(n + n^(2/3)).
 * https://oeis.org/A135673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n + n^(2/3))
 * Range:     u8 n <= 7, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* offset is 1, but guard */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;                /* n^2 */
    seq_t r    = floor_cube_root(n2);        /* floor(n^(2/3)) */
    seq_t c    = floor_cube_root(nseq);      /* cube root of n */
    seq_t cube = c * c * c;                  /* c^3 */
    bool perfect = (cube == nseq);           /* n is a perfect cube? */
    seq_t res = nseq + r + (perfect ? 0 : 1);
    return res;
}
