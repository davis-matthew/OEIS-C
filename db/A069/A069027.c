/*
 * A069027  Powers of 2 with strictly increasing sum of digits.
 * https://oeis.org/A069027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 17, u64 n <= 27, u128 n <= 39,
 *            big any n
 * Helpers:   digit_sum, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) as the nth power of 2 whose decimal digit sum
   is strictly greater than all previous such powers. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is undefined */
        return (seq_t)0;

    seq_t current_power = (seq_t)1;   /* 2^0 */
    seq_t last_sum     = (seq_t)0;    /* digit sum of last accepted power */
    idx_t  count       = 0;           /* number of accepted powers found */

    for (idx_t exp = 0;; ++exp)
    {
        seq_t sum = digit_sum(current_power);

        if (count == 0 || sum > last_sum) {
            count++;
            if (count == n)
                return current_power;
            last_sum = sum;
        }

        /* advance to next power of two */
        current_power = seq_mul(current_power, (seq_t)2);
    }
}
