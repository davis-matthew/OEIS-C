/*
 * A234538  (Number of positive digits of n written in base 3) modulo 3.
 * https://oeis.org/A234538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (# of non‑zero base‑3 digits of n) mod 3
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    idx_t x = n;
    while (x != 0) {
        if (x % 3) ++cnt;
        x /= 3;
    }
    return cnt % (seq_t)3;
}
