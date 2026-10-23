/*
 * A188643  Numbers which are not a multiple of the product of their digits.
 * https://oeis.org/A188643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer that is not a multiple of the
 *            product of its digits. When the product of digits is 0 the
 *            number is automatically accepted
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard */

    idx_t count = 0;
    seq_t cur = 10;                /* first candidate at the OEIS offset */

    while (1) {
        seq_t prod = digit_product(cur);
        if (prod == 0 || (cur % prod != 0)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
