/*
 * A061820  Multiples of 4 containing only digits 0,...,4.
 * https://oeis.org/A061820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Multiples of 4 containing only digits 0…4 (OEIS A061820) */
#include <stdbool.h>

/* Check that all decimal digits of x are in the set {0,1,2,3,4}. */
static inline bool digits_ok(seq_t x)
{
    if (x == 0)
        return true;
    while (x > 0)
    {
        if ((x % 10) > 4)
            return false;
        x /= 10;
    }
    return true;
}

/* Return the n-th term of the sequence, with a(0) = 0. */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  num   = 0;

    while (1)
    {
        if ((num % (seq_t)4) == 0 && digits_ok(num))
        {
            if (count == n)
                return num;
            ++count;
        }
        num += 1;
    }
}
