/*
 * A155902  Arises in Connell's game, a variation of Wythoff's Nim game.
 * https://oeis.org/A155902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*(sqrt(13)+5)/6)
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = (seq_t)13 * nn * nn;
    seq_t m = floor_sqrt(temp);
    return (m + 5 * nn) / 6;
}
