/*
 * A004744  Numbers whose binary expansion does not contain 011.
 * https://oeis.org/A004744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search -- a(n) is the n-th nonnegative integer whose binary
 *            expansion (no leading zeros) does not contain 011
 * Range:     u8 n <= 134, u16 n <= 6747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_011
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_011.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t num = 0;
    for (;;) {
        if (!has_011(num)) {
            count++;
            if (count == n) return (seq_t)num;
        }
        num++;
    }
}
