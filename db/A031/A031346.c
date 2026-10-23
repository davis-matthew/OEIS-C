/*
 * A031346  Multiplicative persistence: number of iterations of "multiply
 *          digits" needed to reach a number < 10.
 * https://oeis.org/A031346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of iterations of the digit-product map needed to
 *            reach a single-digit number
 * Range:     u8 n <= 3825, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>    /* for bool, not mandatory but harmless */
#include "../utils/digit_product.h"  /* declares seq_t digit_product(seq_t) */

seq_t a(idx_t n)
{
    if (n < 10UL)  /* single‑digit numbers have persistence 0 */
        return (seq_t)0;

    seq_t cur = (seq_t)n;
    seq_t count = 0;   /* use seq_t for the count as the return type */

    while (cur >= (seq_t)10) {
        cur = digit_product(cur);
        count++;
    }
    return count;
}
