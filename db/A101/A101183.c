/*
 * A101183  Number of times (>0) function f must be applied in A100800 to
 *          arrive at multiple of n, or 0 if multiple of n is never reached.
 * https://oeis.org/A101183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative search: apply f(n) = n + digit_sum(n) repeatedly
   until the result is divisible by the starting n.
   Return the number of iterations, guaranteed to be >0. */

#include <stdint.h>

/* Compute the sum of decimal digits of a seq_t number. */
static inline seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Starting value */
    seq_t start = (seq_t)n;
    seq_t cur   = start;
    idx_t steps = 0;

    /* Iterate until a multiple of the original n is reached */
    for (;;) {
        ++steps;
        cur += digit_sum(cur);
        if (cur % start == 0)
            break;
    }

    return (seq_t)steps;
}
