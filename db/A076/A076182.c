/*
 * A076182  a(n) = A006666(n) mod 2.
 * https://oeis.org/A076182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006666(n) mod 2, where A006666(n) is the number of steps
 *            of T(x) = x/2 (x even), (3x+1)/2 (x odd) needed to reach 1 from
 *            n
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9949 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    unsigned parity = 0;

    while (x != 1) {
        if ((x & 1) == 0) {
            x = x / 2;
        } else {
            seq_t h = x / 2;
            x = 3 * h + 2;   /* (3x+1)/2 for odd x */
        }
        parity ^= 1u;
    }

    return (seq_t)parity;
}
