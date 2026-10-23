/*
 * A136274  Decimal expansion of 1/998999.
 * https://oeis.org/A136274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit of 1/998999 after the decimal
 *            point (OEIS A136274)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 103, u64 n <= 103, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A static cache of already computed digits */
    static seq_t *digits   = NULL;   /* array of digits */
    static size_t  length  = 0;      /* number of digits stored so far */
    static size_t  capacity= 0;      /* current allocated capacity */
    static seq_t  remainder = 1;     /* remainder after the last computed digit */
    static idx_t   next_index=0;     /* the index of the next digit to compute */

    const seq_t D = 998999;          /* denominator */

    /* Allocate initial storage if necessary */
    if (digits == NULL) {
        capacity = 128;
        digits = (seq_t *)malloc(capacity * sizeof(seq_t));
        length = 0;
    }

    /* If the requested digit is already computed, return it */
    if (n < length)
        return digits[n];

    /* Compute digits up to n inclusive */
    while (next_index <= n) {
        remainder *= 10;                     /* bring down next decimal place */
        seq_t digit = remainder / D;
        remainder  = remainder % D;

        /* Grow the array if needed */
        if (length >= capacity) {
            capacity *= 2;
            digits = (seq_t *)realloc(digits, capacity * sizeof(seq_t));
        }

        digits[length++] = digit;
        next_index++;
    }

    return digits[n];
}
