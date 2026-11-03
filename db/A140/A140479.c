/*
 * A140479  n^2 - number of digits of n^2.
 * https://oeis.org/A140479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^2 - A055642(n^2) (n^2 minus the number of decimal
 *            digits of n^2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 970 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t sq = (seq_t)n * (seq_t)n;
    int d = num_digits(sq);
    return sq - (seq_t)d;
}
