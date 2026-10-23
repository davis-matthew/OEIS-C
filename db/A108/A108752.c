/*
 * A108752  Numbers k such that 12 divides k*(k+1).
 * https://oeis.org/A108752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*((n-1)/4) + [0,3,8,11][(n-1)%4]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1; value for n=0 not used */
    idx_t m = n - 1;                    /* zero‑based index */
    idx_t t = m / 4;                     /* block number */
    idx_t r = m % 4;                     /* position within the block */
    static const seq_t offsets[4] = {0, 3, 8, 11};
    return (seq_t)(12 * t) + offsets[r];
}
