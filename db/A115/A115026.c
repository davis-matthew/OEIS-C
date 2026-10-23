/*
 * A115026  Limiting value of n under iteration of "sum of the digits raised
 *          to the power of the number of digits of n" (A101337).
 * https://oeis.org/A115026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = limiting value obtained by iterating f(n) = Σ d_i^k *
 *            where d_i are the decimal digits of n and k is the number of
 *            digits. * The returned value is the smallest element of the
 *            cycle that is * reached, or the fixed point if the orbit
 *            stabilizes
 * Range:     u8 n <= 13, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_power_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_power_sum.h"

/* Main sequence function.  Uses Floyd’s cycle detection to avoid
 * dynamic memory allocation and to support arbitrary–precision seq_t. */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;       /* initial value */

    /* 1. Find a repetition of values: tortoise moves one step,
     *    hare moves two steps.  When they meet we have a cycle. */
    seq_t tortoise = digit_power_sum(start);
    seq_t hare = digit_power_sum(digit_power_sum(start));
    while (tortoise != hare) {
        tortoise = digit_power_sum(tortoise);
        hare = digit_power_sum(digit_power_sum(hare));
    }

    /* 2. Find the first element of the cycle (mu). */
    tortoise = start;
    while (tortoise != hare) {
        tortoise = digit_power_sum(tortoise);
        hare = digit_power_sum(hare);
    }

    /* 3. Find the minimal element in the cycle. */
    seq_t min = tortoise;
    seq_t cur = digit_power_sum(tortoise);
    while (cur != tortoise) {
        if (cur < min)
            min = cur;
        cur = digit_power_sum(cur);
    }
    return min;
}
