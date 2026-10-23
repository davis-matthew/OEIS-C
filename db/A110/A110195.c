/*
 * A110195  a(n) = 11^((n^2-n)/2).
 * https://oeis.org/A110195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^((n^2 - n)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the exponent is 0, so the result is 1. */
    if (n <= 1)
        return 1;

    /* Compute e = n*(n-1)/2.  Use idx_t to avoid overflow on n. */
    idx_t e = n * (n - 1) / 2;

    seq_t result = 1;   /* 11^0 */
    seq_t base   = 11;  /* base for exponentiation */

    /* Exponentiation by squaring.  All arithmetic is on seq_t to
       work with arbitrary‑precision or native integer types. */
    while (e) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e)          /* avoid unnecessary squaring at the end */
            base *= base;
    }

    return result;
}
