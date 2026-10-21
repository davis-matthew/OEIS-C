/*
 * A221016  The Wiener index of the fullerenyl anion G[n]C[60], defined
 *          pictorially in the Arezoomand reference (see Fig. 5).
 * https://oeis.org/A221016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10172 + 2^n*(5600*n + 4880) + 4^n*(1280*n - 2624)
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 24, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t two   = pow2(n);
    seq_t four  = two * two;
    seq_t part1 = 5600 * tn + 4880;
    seq_t part2 = 1280 * tn - 2624;
    seq_t res   = 10172 + two * part1 + four * part2;
    return res;
}
