/*
 * A117589  Periodic with period 8: repeat 0, 1, 3, 7, 15, 14, 12, 8.
 * https://oeis.org/A117589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period 8: a(n) = [0,1,3,7,15,14,12,8][n mod 8] */

#include <stddef.h>

static const seq_t _A117589_period[8] = {0, 1, 3, 7, 15, 14, 12, 8};

seq_t a(idx_t n)
{
    return _A117589_period[(size_t)(n & 7)];
}
