/*
 * A037312  Numbers whose base-3 and base-7 expansions have the same digit
 *          sum.
 * https://oeis.org/A037312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑3 and base‑7 digit sums are equal */
#include <stdbool.h>

/* sum of digits of value in the supplied base (base must fit in seq_t) */
static inline seq_t digit_sum_base(seq_t value, seq_t base)
{
    seq_t sum = 0;
    while (value)
    {
        sum += value % base;
        value /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t x = 1;          /* start from 1, as the sequence is offset 1 */
    idx_t count = 0;      /* how many matches we have found so far */

    for (;; ++x)
    {
        if (digit_sum_base(x, (seq_t)3) == digit_sum_base(x, (seq_t)7))
        {
            ++count;
            if (count == n)
                return x;
        }
    }
}
