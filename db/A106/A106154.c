/*
 * A106154  Generation 5 of the substitution 1->{2, 1, 2}, 2->{3, 2, 3},
 *          3->{4, 3, 4}, 4->{5, 4, 5}, 5->{6, 5, 6}, 6->{1, 6, 1}, starting
 *          with 1.
 * https://oeis.org/A106154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (1 + #{digits among the 5 least significant base‑3 digits
 *            of n that are 0 or 2}) mod 6
 * Range:     u8 n <= 242, u16 n <= 242, u32 n <= 242, u64 n <= 242,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of increments (digits 0 or 2) */
    for (int i = 0; i < 5; ++i) {
        idx_t d = n % 3;      /* least significant base‑3 digit */
        if (d != 1) ++count;  /* 0 or 2 triggers an increment */
        n /= 3;
    }
    return (seq_t)((count % 6) + 1);
}
