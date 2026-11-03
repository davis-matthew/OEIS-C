/*
 * A084682  Even evil numbers with an even digital sum.
 * https://oeis.org/A084682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 8196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9957 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Even evil numbers with an even digital sum:
   a(1) = 0
   For n ≥ 1, a(n) is the n‑th even integer x such that
   (i)  x has an even number of 1‑bits in its binary representation
   (ii) the sum of the decimal digits of x is even. */
#include <stdbool.h>

/* Check whether the sum of decimal digits of x is even. */
static bool even_digit_sum(seq_t x)
{
    seq_t y = x;
    int sum = 0;
    while (y) {
        sum += (int)(y % 10);
        y /= 10;
    }
    return (sum & 1) == 0;   /* even sum */
}

/* Check whether x is an even evil number (even number of 1‑bits and x is even). */
static bool is_evil_even(seq_t x)
{
    /* Must be even */
    if ((x & 1) != 0)
        return false;

    /* Compute parity of 1‑bits */
    seq_t y = x;
    int parity = 0;          /* 0 = even, 1 = odd */
    while (y) {
        parity ^= 1;
        y &= y - 1;          /* clear lowest set bit */
    }
    return parity == 0;
}

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
