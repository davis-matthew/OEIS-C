/*
 * A037751  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1,0.
 * https://oeis.org/A037751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digits 2,3,1,0 repeating interpreted as a base‑4 number
 *            of length n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, a(0) is unused */
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        switch (i & 3u) {           /* i mod 4 using bit mask for speed */
            case 0: res = res * 4 + 2; break;
            case 1: res = res * 4 + 3; break;
            case 2: res = res * 4 + 1; break;
            case 3: res = res * 4 + 0; break;
        }
    }
    return res;
}
