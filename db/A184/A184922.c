/*
 * A184922  a(n) = n + [rn/t] + [sn/t] + [un/t], where []=floor and r=2^(1/2),
 *          s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A184922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(2+sqrt(2))) - 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          // n^2
    seq_t two_sq = sq * 2;       // 2*n^2
    seq_t s = floor_sqrt(two_sq);    // floor(sqrt(2*n^2)) = floor(n*sqrt2)
    seq_t res = nn * 2 - 1 + s; // 2n - 1 + floor(n*sqrt2)
    return res;
}
