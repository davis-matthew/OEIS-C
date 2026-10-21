/*
 * A208273  Composite numbers containing a digit 2.
 * https://oeis.org/A208273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth composite number that contains the digit 2
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)4;      /* first composite greater than 3 */
    idx_t cnt = 0;

    while (1)
    {
        /* check if num contains the digit 2 */
        bool has_two = false;
        seq_t tmp = num;
        while (tmp > (seq_t)0)
        {
            if ((tmp % (seq_t)10) == (seq_t)2)
            {
                has_two = true;
                break;
            }
            tmp /= (seq_t)10;
        }

        if (has_two && !is_prime_big(num))
        {
            ++cnt;
            if (cnt == n)
                return num;
        }
        ++num;
    }
}
