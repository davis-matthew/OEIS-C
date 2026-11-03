/*
 * A127423  a(1) = 1; for n > 1, a(n) = n concatenated with n - 1.
 * https://oeis.org/A127423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(n)=concat(n, n-1) for n>1
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9042 (2026-11-01)
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
    if (n == 1)
        return (seq_t)1;
    return concat_parts(n, n - 1);
}
