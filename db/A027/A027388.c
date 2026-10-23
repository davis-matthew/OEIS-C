/*
 * A027388  Write digits for n, count endpoints (version 3).
 * https://oeis.org/A027388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(10*n + d) = a(n) + e(d) where
   e(0)=e(8)=0, e(6)=e(9)=1, e(1,2,3,4,5)=2, e(7)=4.
   a(0) = 0. */
#include <stdint.h>

/* Endpoint counts for digits 0–9 */
static const unsigned char endpoint[10] = {
    0, 2, 2, 2, 2, 2, 1, 4, 0, 1
};

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n > 0) {
        unsigned d = n % 10;
        res += endpoint[d];
        n /= 10;
    }
    return res;
}
