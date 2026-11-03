/*
 * A062704  Di-Boustrophedon transform of all 1's sequence: Fill in an array
 *          by diagonals alternating in the 'up' and 'down' directions. Each
 *          diagonal starts with a 1. When going in the 'up' direction the
 *          next element is the sum of the previous element of the diagonal
 *          and the previous two elements of the row...
 * https://oeis.org/A062704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 161 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062704: the di-boustrophedon transform of all 1s. Diagonal d (r + c = d)
 * starts with 1 and alternates direction: odd d goes up from (d,0) to (0,d),
 * each new entry adding the previous entry of the diagonal and the (at most)
 * two entries just before it in its row; even d goes down from (0,d) to (d,0),
 * using the two entries just above it in its column. a(n) is the last entry of
 * diagonal n-1. Sums only, so exact modulo the width. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const size_t D = (size_t)n;               /* diagonals 0 .. n-1 */
    seq_t *T = mem_new(seq_t, D * D);         /* T[r * D + c] */
    if (!T) abort();
    seq_t last = (seq_t)1;
    T[0] = (seq_t)1;
    for (size_t d = 1; d < D; ++d) {
        if (d & 1) {                          /* up: rows d .. 0 */
            T[d * D] = (seq_t)1;
            for (size_t c = 1; c <= d; ++c) {
                size_t r = d - c;
                seq_t v = T[(r + 1) * D + c - 1];
                v += T[r * D + c - 1];
                if (c >= 2) v += T[r * D + c - 2];
                T[r * D + c] = v;
            }
            last = T[d];
        } else {                              /* down: rows 0 .. d */
            T[d] = (seq_t)1;
            for (size_t r = 1; r <= d; ++r) {
                size_t c = d - r;
                seq_t v = T[(r - 1) * D + c + 1];
                v += T[(r - 1) * D + c];
                if (r >= 2) v += T[(r - 2) * D + c];
                T[r * D + c] = v;
            }
            last = T[d * D];
        }
    }
    mem_free(T);
    return last;
}
