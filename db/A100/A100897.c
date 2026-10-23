/*
 * A100897  a(n) is the decimal expansion of 7nn7.
 * https://oeis.org/A100897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7nn7
 * Range:     u8 none, u16 n <= 9, u32 n <= 999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits.h"

seq_t a(idx_t n) {
    int d = digits(n);
    seq_t pow10 = (seq_t)1;
    for (int i = 0; i < d; ++i) pow10 *= (seq_t)10;

    seq_t part1 = (seq_t)7 * pow10 + (seq_t)n;
    seq_t part2 = part1 * pow10 + (seq_t)n;
    seq_t result = part2 * (seq_t)10 + (seq_t)7;
    return result;
}
