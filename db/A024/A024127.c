/*
 * A024127  a(n) = 11^n-1.
 * https://oeis.org/A024127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 1 Recurrence: a(0) = 0, a(n) = 11*a(n-1) + 10 for
 *            n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If n is 0 the loop below would skip, so we return 0 directly. */
    if (n == 0) return (seq_t)0;

    /* Binary exponentiation to compute 11^n in O(log n) multiplies.  */
    seq_t result = (seq_t)1;   /* 11^0  */
    seq_t base   = (seq_t)11;  /* 11^1  */

    while (n > 0) {
        if (n & 1)          /* if the current bit is set */
            result *= base; /* multiply by current power of 11     */

        base *= base;       /* square the base: 11^{2^k}          */
        n >>= 1;            /* move to next bit in exponent       */
    }

    /* a(n) = 11^n - 1; for n > 0, result >= 1 so subtraction is safe. */
    return result - (seq_t)1;
}
