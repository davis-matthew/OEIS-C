/*
 * A188070  Positions of 1 in A188068; complement of A188069.
 * https://oeis.org/A188070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n * (1 + sqrt(3)) / 2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t nseq = (seq_t)n;
    /* compute floor_sqrt(3 * n^2) */
    seq_t n2   = nseq * nseq;            /* n^2 */
    seq_t prod = (seq_t)3 * n2;          /* 3 * n^2 */
    seq_t s    = floor_sqrt(prod);       /* floor(sqrt(3 * n^2)) */

    /* a(n) = floor( (n + floor_sqrt(3*n*n)) / 2 ) + 1 */
    seq_t res = (nseq + s) / 2 + 1u;
    return res;
}
