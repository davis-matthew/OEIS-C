/*
 * A001481  Numbers that are the sum of 2 squares.
 * https://oeis.org/A001481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, core, changed
 * Formula:   search: a(n) is the n-th k >= 0 in which every prime p == 3 (mod
 *            4) divides k to an even power
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001481_is_s2s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a001481_is_s2s.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t num = 0;
    for (;;) {
        if (a001481_is_s2s(num)) {
            count++;
            if (count == n) return (seq_t)num;
        }
        num++;
    }
}
