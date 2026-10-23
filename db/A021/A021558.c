/*
 * A021558  Decimal expansion of 1/554.
 * https://oeis.org/A021558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th decimal digit (0‑based) of 1/554
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Denominator of the fraction */
    const uint64_t denom = 554ULL;

    /* Static state for memoization */
    static seq_t *digits = NULL;     /* array of computed digits           */
    static size_t  cap     = 0;      /* allocated size in elements         */
    static idx_t   computed = 0;     /* number of digits already computed */
    static uint64_t rem     = 1;     /* current remainder after last digit */

    /* Grow the array if necessary (no resizing beyond n). */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 as a sentinel. */
            return (seq_t)0;
        }
        digits = tmp;
        cap = new_cap;
    }

    /* Produce any missing digits up to n by long division. */
    for (idx_t i = computed; i <= n; ++i) {
        rem *= 10ULL;                     /* bring next decimal digit      */
        seq_t digit = (seq_t)(rem / denom);/* integer part of the division */
        digits[i] = digit;                /* store the digit               */
        rem %= denom;                     /* remainder for next step       */
    }
    computed = n + 1;   /* all digits up to n are now known */

    return digits[(size_t)n];
}
