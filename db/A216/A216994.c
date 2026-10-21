/*
 * A216994  Multiples of 7 such that the digit sum is divisible by 7.
 * https://oeis.org/A216994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Multiples of 7 whose digit sum is divisible by 7 (OEIS A216994)
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Local helper: compute the digit sum of a number in seq_t */
static seq_t digit_sum_local(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but guard anyway */

    seq_t multiple = (seq_t)7;    /* the first possible term */
    idx_t found   = 0;            /* how many terms found so far */

    while (1) {
        if (digit_sum_local(multiple) % 7 == 0) {
            ++found;
            if (found == n)
                return multiple;
        }
        multiple += (seq_t)7;     /* next candidate */
    }
}
