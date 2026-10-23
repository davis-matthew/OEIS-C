/*
 * A037333  Numbers whose base-7 and base-10 expansions have the same digit
 *          sum.
 * https://oeis.org/A037333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose decimal- and base-7 digit
 *            sums are equal
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Computes digit sum of a number in base 7 */
static inline seq_t digit_sum_base7(seq_t x)
{
    seq_t sum = 0;
    while (x != 0)
    {
        sum += x % 7;
        x /= 7;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1, but guard against invalid call */

    idx_t found = 0;
    seq_t num  = 1;

    while (true)
    {
        seq_t sum_dec = digit_sum(num);     /* decimal digit sum */
        seq_t sum_b7  = digit_sum_base7(num);/* base‑7 digit sum */

        if (sum_dec == sum_b7)
        {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
}
