/*
 * A168558  a(n) = n + a(n-1)^2 for n>1, a(1)=0.
 * https://oeis.org/A168558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + a(n-1)^2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 0;
    if (n == 1) {
        return prev;
    }
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sq   = prev * prev;
        seq_t iv   = (seq_t)i;
        prev       = iv + sq;
    }
    return prev;
}
