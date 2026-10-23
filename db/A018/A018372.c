/*
 * A018372  Divisors of 282.
 * https://oeis.org/A018372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 282: a(n) is the n-th divisor of 282 in ascending order */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 282 (= 2 * 3 * 47) has exactly 8 divisors.  Return 0 for n > 8. */
    if (n <= 0 || n > 8)
        return (seq_t)0;

    seq_t count = 0;
    for (idx_t d = 1; d <= (idx_t)282; ++d) {
        if ((seq_t)282 % d == 0) {
            ++count;
            if (count == n)
                return (seq_t)d;
        }
    }
    return (seq_t)0;
}
