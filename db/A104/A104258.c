/*
 * A104258  Replace 2^i with n^i in binary representation of n.
 * https://oeis.org/A104258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i: bit i of n is 1} n^i (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 255,
 *            u128 n <= 3183, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset 1, but guard anyway */
    seq_t base = (seq_t)n;           /* base for powers */
    seq_t pow  = 1;                  /* n^0 */
    seq_t result = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            result += pow;
        }
        tmp >>= 1;
        pow *= base;
    }
    return result;
}
