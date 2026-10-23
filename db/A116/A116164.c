/*
 * A116164  a(n) = 6^n * n*(n+1).
 * https://oeis.org/A116164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n * n*(n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i) {
        power *= (seq_t)6;
    }
    seq_t sn = (seq_t)n;
    return power * sn * (sn + 1);
}
