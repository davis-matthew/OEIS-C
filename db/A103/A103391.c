/*
 * A103391  "Even" fractal sequence for the natural numbers: Deleting every
 *          even-indexed term results in the same sequence.
 * https://oeis.org/A103391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1. For n > 1, let t = n – 1. Write t = 2^k * odd, with
 *            odd odd. Then a(n) = (odd + 3) / 2. (All operations are on
 *            unsigned integers.)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Remove all factors of 2 from n-1 to get the odd part. */
    idx_t t = n - 1;
    while ((t & (idx_t)1) == 0)
        t >>= 1;                /* t is now odd */

    /* Compute the value: (odd + 3) / 2.  The intermediate is a idx_t. */
    idx_t value_idx = (t + (idx_t)3) / (idx_t)2;

    return (seq_t)value_idx;
}
