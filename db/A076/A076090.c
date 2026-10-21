/*
 * A076090  a(1) = 1; for n > 1, smallest number > 1 which is the n-th power
 *          of the sum of its digits, or 0 if no such number exists.
 * https://oeis.org/A076090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A046017(n)^n, with a(1) = 1 A046017(n) is the smallest
 *            integer b>1 such that digit_sum(b^n) == b. If no such b exists,
 *            a(n) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"

#define BASE_SEARCH_LIMIT 100000ULL  /* empirical upper bound for the base */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    for (idx_t b = 2; b <= BASE_SEARCH_LIMIT; ++b)
    {
        seq_t pow_val = big_pow((seq_t)b, n);
        /* If the power is zero (possible on overflow in some build),
           terminate the search. */
        if (pow_val == 0) return (seq_t)0;

        seq_t sum = digit_sum(pow_val);
        if (sum == (seq_t)b) return pow_val;
    }
    return (seq_t)0;
}
