/*
 * A085444  Numbers divisible by twice the sum of the products of each of
 *          their digits.
 * https://oeis.org/A085444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth positive integer k such that k % ( 2 *
 *            (digit_sum(k))^2 ) == 0
 * Range:     u8 n <= 1, u16 n <= 238, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A085444: numbers divisible by twice the square of the sum of their digits */

#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 2;                 /* all terms are even, so start at 2 */

    while (true) {
        seq_t s = digit_sum(x);

        /* d = 2 * (s^2) */
        seq_t d = s * s;         /* square of digit sum */
        d += d;                  /* multiply by 2 */

        if (x % d == 0) {
            if (found == n)
                return x;
            ++found;
        }

        x += 2;                  /* skip odd numbers – they can never be divisible by even d */
    }
}
