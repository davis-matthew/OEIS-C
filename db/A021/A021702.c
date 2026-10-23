/*
 * A021702  Decimal expansion of 1/698.
 * https://oeis.org/A021702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/698 at position n
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
    /* static state for memoisation */
    static seq_t *digits = NULL;
    static size_t digits_cap = 0;
    static idx_t cached_next = 0;
    static uint64_t rem = 1;          /* remainder of ongoing division */

    /* If the requested digit is already cached, return it */
    if (n < cached_next)
        return digits[n];

    /* Initialise storage on first call */
    if (digits == NULL) {
        digits_cap = 16;
        digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > digits_cap) {
        size_t new_cap = digits_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                 /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        digits = tmp;
        digits_cap = new_cap;
    }

    /* Compute digits from the last cached position up to n */
    for (idx_t i = cached_next; i <= n; ++i) {
        rem *= 10;                       /* bring the next decimal place */
        seq_t digit = (seq_t)(rem / 698);/* integer part of the division */
        rem %= 698;                      /* remaining fractional part */
        digits[i] = digit;
    }
    cached_next = n + 1;                  /* next index to compute */

    return digits[n];
}
