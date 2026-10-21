/*
 * A069030  Powers of 5 with strictly increasing sum of digits.
 * https://oeis.org/A069030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th power of 5 whose digit sum strictly increases
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   digit_sum, mul_small, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"
#include "../utils/digit_sum.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* start with 5^0 = 1 */
    seq_t power = power5(0);
    seq_t last_sum = 0;

    idx_t count = 0;
    for (idx_t exp = 0; ; ++exp)
    {
        seq_t sum = digit_sum(power);
        if (sum > last_sum)
        {
            ++count;
            if (count == n)
                return power;          /* found the n-th term */
            last_sum = sum;            /* update previous sum */
        }
        /* advance to the next power of 5 */
        power = mul_small(power, 5);
    }
}
