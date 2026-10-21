/*
 * A212284  First a(n) > 1 whose sum of digits is the same in base 10 as in
 *          base n.
 * https://oeis.org/A212284
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 2221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = first integer m ≥ n (m > 1) such that
          digit_sum(m,10) == digit_sum(m,n),
   where digit_sum(x,b) is the sum of the digits of x when written in base b.
   The function searches sequentially starting at m = n, and returns 0
   if the search overflows the seq_t type (no solution found within the range).
*/

#include <stdbool.h>

static seq_t digit_sum(seq_t value, idx_t base)
{
    seq_t b = (seq_t)base;
    seq_t sum = 0;
    while (value != 0)
    {
        sum += value % b;
        value /= b;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* out of defined range */

    seq_t m = (seq_t)n;              /* start at m = n */
    while (true)
    {
        if (digit_sum(m, 10) == digit_sum(m, n))
            return m;
        /* avoid infinite loop if seq_t overflows */
        if (m == 0)
            return 0;
        ++m;
    }
}
