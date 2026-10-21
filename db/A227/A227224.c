/*
 * A227224  Numbers k such that k*(sum of digits of k) is a perfect square.
 * https://oeis.org/A227224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227224: numbers k such that k * digit_sum(k) is a perfect square */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k) {
        seq_t ks   = (seq_t)k;                /* current number as seq_t      */
        seq_t ssum = digit_sum(ks);           /* sum of digits of ks          */
        seq_t prod = ks * ssum;               /* k * sumdigits                */
        seq_t root = floor_sqrt(prod);        /* integer floor of sqrt(prod)  */
        if (root * root == prod) {            /* perfect square check         */
            ++count;
            if (count == n)
                return ks;                    /* found the nth term            */
        }
    }
}
