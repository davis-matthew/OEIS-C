/*
 * A133016  Even imperfect numbers, divided by 2.
 * https://oeis.org/A133016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th even number that is not a perfect number, divided by
 *            2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_even_perfect
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_even_perfect.h"

seq_t a(idx_t n)
{
    /* n is 1-based index */
    idx_t i = 1;       /* current k (half of the even number) */
    idx_t count = 0;   /* how many even imperfect numbers have been seen */

    for (;; ++i) {
        seq_t value = ((seq_t)i) * ((seq_t)2);  /* the even number 2*i */
        if (!is_even_perfect(value)) {
            ++count;
            if (count == n)
                return (seq_t)i;               /* return k = a(n) */
        }
    }
}
