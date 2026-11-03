/*
 * A239459  Concatenation of n^3 and n.
 * https://oeis.org/A239459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_parts(n^3, n)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 49 terms through n = 893 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

seq_t a(idx_t n)
{
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;
    return concat_parts((idx_t)n3, n);
}
