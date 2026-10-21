/*
 * A062040  Positive numbers whose product of digits is 8 times their sum.
 * https://oeis.org/A062040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 268, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: sequentially enumerate positive integers and return the nth
 * that satisfies the condition product_of_digits = 8 * sum_of_digits. */

#include <stdbool.h>

/* Helper: check whether a number satisfies the condition.
 * Digits are processed from least to most significant.  A zero digit
 * automatically disqualifies the number because the product becomes 0
 * while the sum is positive.  The product is accumulated in an
 * unsigned __int128 to avoid overflow for moderately long numbers.
 */
static bool satisfies(seq_t n)
{
    if (n == 0)
        return false;

    unsigned __int128 prod = 1;
    uint64_t sum = 0;
    seq_t tmp = n;

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        if (d == 0)          /* product would be 0, impossible to equal 8*sum */
            return false;

        prod *= d;
        sum += d;

        tmp /= 10;
    }

    return prod == (unsigned __int128)8 * sum;
}

/* Main routine: return a(n) for the OEIS sequence A062040. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 1;

    for (;;) {
        if (satisfies(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
