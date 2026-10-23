/*
 * A021989  Decimal expansion of 1/985.
 * https://oeis.org/A021989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th decimal digit after the point of 1/985. The
 *            function builds the digits by long division, keeping a cache so
 *            that repeated calls in any order are efficient
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Returns the n-th digit (0‑based) in the decimal expansion of 1/985. */
seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *digits = NULL;
    static size_t  len    = 0;          /* number of digits stored */
    static size_t  cap    = 0;          /* capacity of the array */
    static unsigned int rem = 1;        /* remainder after last digit, 1 <= rem < 985 */

    /* ensure we have enough space for digit n */
    while (len <= (size_t)n) {
        /* grow the cache if necessary */
        if (cap == 0) {
            cap = 16;
            digits = (seq_t*)malloc(cap * sizeof(seq_t));
        } else if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp     = (seq_t*)realloc(digits, new_cap * sizeof(seq_t));
            digits = tmp;
            cap    = new_cap;
        }

        /* compute next digit using long division */
        unsigned int tmp = rem * 10U;      /* temporary product, fits in 32 bits */
        seq_t    digit   = (seq_t)(tmp / 985U);
        rem  = tmp % 985U;                /* update remainder for next iteration */

        digits[len++] = digit;
    }

    return digits[(size_t)n];
}
