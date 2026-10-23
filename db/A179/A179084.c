/*
 * A179084  Odd numbers having an even sum of digits in their decimal
 *          representation.
 * https://oeis.org/A179084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return a(n): the nth odd number whose decimal digit sum is even. */

#include <stdbool.h>
#include "../utils/digit_sum.h"

static bool has_even_digit_sum(seq_t x)
{
    seq_t sum = digit_sum(x);
    seq_t zero = 0;
    return ((sum % 2) == zero);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0; sentinel */

    idx_t  count   = 0;
    seq_t  cand    = 1;              /* start with the smallest odd number */

    for (;;) {
        if (has_even_digit_sum(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand += 2;                   /* next odd number */
    }
}
