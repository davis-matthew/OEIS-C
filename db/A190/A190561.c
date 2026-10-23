/*
 * A190561  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,3) and
 *          []=floor.
 * https://oeis.org/A190561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*n+3)*sqrt(2)) - 4*floor(n*sqrt(2)) - 4
 * Range:     u8 none, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor((4*n+3)*sqrt(2)) */
    idx_t t = (idx_t)4 * n + 3;
    /* 2*t*t may fit in 64‑bit for typical inputs (tests up to 10k) */
    idx_t tt = (idx_t)2 * t * t;
    seq_t first = floor_sqrt((seq_t)tt);

    /* Compute floor(n*sqrt(2)) */
    idx_t nn = (idx_t)2 * n * n;
    seq_t second = floor_sqrt((seq_t)nn);

    /* Final a(n) value */
    return first - (seq_t)4 * second - (seq_t)4;
}
