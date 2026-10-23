/*
 * A021959  Decimal expansion of 1/955.
 * https://oeis.org/A021959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1))/955) mod 10, i.e. the n-th decimal digit
 *            of 1/955, with offset 0
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t rem = 1; /* start with 1 before first digit */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        seq_t digit = rem / ((seq_t)955);
        rem = rem % ((seq_t)955);
        if (i == n)
            return digit;
    }
    return 0; /* unreachable, satisfies compiler */
}
