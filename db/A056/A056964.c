/*
 * A056964  a(n) = n + reversal of digits of n.
 * https://oeis.org/A056964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + reverse_digits(n)
 * Range:     u8 n <= 101, u16 n <= 10005, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 81 terms through n = 9891 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x + reverse_digits(x);
}
