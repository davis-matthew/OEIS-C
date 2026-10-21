/*
 * A080480  Largest number formed by using all the digits (with multiplicity)
 *          of next n numbers.
 * https://oeis.org/A080480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest number formed by using all the digits of the next n consecutive
   integers starting after the (n−1)‑st triangular number.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the first number of the block, T_{n-1}+1 */
    idx_t triangular = (n - 1) * n / 2;
    idx_t start = triangular + 1;

    unsigned int cnt[10] = {0};

    /* Count digits of all n numbers */
    for (idx_t i = 0; i < n; ++i)
    {
        idx_t num = start + i;
        /* numbers are positive, so no special 0 case */
        while (num > 0)
        {
            unsigned int d = (unsigned int)(num % 10);
            ++cnt[d];
            num /= 10;
        }
    }

    /* Build the largest number by concatenating digits in descending order */
    seq_t result = (seq_t)0;
    for (int d = 9; d >= 0; --d)
    {
        for (unsigned int c = cnt[d]; c > 0; --c)
        {
            result = result * (seq_t)10 + (seq_t)d;
        }
    }
    return result;
}
