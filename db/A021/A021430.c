/*
 * A021430  Decimal expansion of 1/426.
 * https://oeis.org/A021430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth digit (0‑based) of the decimal expansion of 1/426
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed digits and the current remainder. */
    static seq_t *digits = NULL;          /* array of digits */
    static idx_t  digits_len = 0;         /* number of digits stored so far */
    static size_t digits_cap = 0;         /* allocated array capacity */
    static seq_t  rem = 1;                /* current remainder before next digit */
    static const seq_t divisor = (seq_t)426;  /* constant divisor */

    /* Ensure the cache can hold up to index n.  Grow if necessary. */
    if ((idx_t)n >= digits_len) {
        size_t required = (size_t)(n + 1);
        if (required > digits_cap) {
            size_t new_cap = digits_cap ? digits_cap * 2 : 16;
            while (new_cap < required)
                new_cap *= 2;
            seq_t *tmp = NULL;

#if defined(__cplusplus)
            /* C++: allocate with new[] (which calls constructors). */
            tmp = new seq_t[new_cap];
            /* Copy existing digits into the new array. */
            for (size_t i = 0; i < digits_len; ++i)
                tmp[i] = digits[i];
            /* Release the old array. */
            delete[] digits;
#else
            /* C: allocate with realloc (seq_t is an integral type). */
            tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
#endif
            /* If reallocation fails, keep the old array (remains valid). */
            if (tmp) {
                digits = tmp;
                digits_cap = new_cap;
            }
        }

        /* Compute any new digits up to n. */
        for (idx_t i = digits_len; i <= n; ++i) {
            rem = rem * (seq_t)10;
            seq_t digit = rem / divisor;
            rem = rem % divisor;
            digits[i] = digit;
        }
        digits_len = (idx_t)(n + 1);
    }

    return digits[n];
}
