/*
 * A084682  Even evil numbers with an even digital sum.
 * https://oeis.org/A084682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 8196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_digit_sum, is_evil_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Even evil numbers with an even digital sum:
   a(1) = 0
   For n ≥ 1, a(n) is the n‑th even integer x such that
   (i)  x has an even number of 1‑bits in its binary representation
   (ii) the sum of the decimal digits of x is even. */
#include <stdbool.h>
#include "../utils/even_digit_sum.h"
#include "../utils/is_evil_even.h"

/* Return the n‑th even evil number with an even digit sum. */
seq_t a(idx_t n)
{
    seq_t cur = 0;          /* start from 0 */
    idx_t count = 0;        /* number of terms found so far */

    while (count < n) {
        if (is_evil_even(cur) && even_digit_sum(cur)) {
            count++;
            if (count == n)
                break;
        }
        cur += 2;           /* only even numbers need be checked */
    }
    return cur;
}
