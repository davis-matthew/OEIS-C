/*
 * A021196  Decimal expansion of 1/192.
 * https://oeis.org/A021196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the (n+1)-th decimal digit of 1/192
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow out-of-order access while keeping
       linear computation time. */
    static seq_t *digits = NULL;       /* array of computed digits */
    static size_t capacity = 0;        /* allocated size of 'digits' */
    static size_t count = 0;           /* number of digits already computed */
    static uint32_t rem = 1;           /* current remainder (0 <= rem < 192) */

    /* Initialise the cache on the first call. */
    if (digits == NULL) {
        capacity = 64;
        digits = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (digits == NULL) { return (seq_t)0; } /* guard against OOM */
    }

    /* Generate digits up to index n. */
    while (count <= n) {
        rem *= 10u;                                 /* shift remainder */
        seq_t digit = (seq_t)(rem / 192u);           /* next decimal digit */
        rem %= 192u;                                /* update remainder */

        /* Grow the cache if necessary. */
        if (count >= capacity) {
            capacity *= 2u;
            seq_t *new_digits = (seq_t *)realloc(digits, capacity * sizeof(seq_t));
            if (new_digits == NULL) { return (seq_t)0; } /* OOM guard */
            digits = new_digits;
        }

        digits[count] = digit;
        ++count;
    }

    return digits[n];
}
