/*
 * A229093  The clubs patterns appearing in n X n coins.
 * https://oeis.org/A229093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((n-1)^2/3) with a(0)=0 and a(4)=4
 * Range:     u8 n <= 28, u16 n <= 444, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* compute (n-1)^2 + 2 then divide by 3 (integer division truncates) */
    idx_t nm1 = n - 1;
    idx_t d   = nm1 * nm1 + 2;   /* d fits in 64‑bit for the required range */
    seq_t res = (seq_t)(d / 3);
    if (n == 4) res = 4;
    return res;
}
