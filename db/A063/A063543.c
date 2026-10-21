/*
 * A063543  a(n) = n - product of the nonzero digits of n.
 * https://oeis.org/A063543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n - product of the nonzero digits of n
 * Range:     u8 n <= 269, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;          /* product of non‑zero digits */
    idx_t m = n;             /* work on a copy of n */

    while (m > 0) {
        unsigned digit = m % 10; /* current decimal digit */
        if (digit != 0)
            prod *= digit;      /* multiply into seq_t product */
        m /= 10;
    }

    return (seq_t)n - prod;  /* difference fits in seq_t by definition */
}
