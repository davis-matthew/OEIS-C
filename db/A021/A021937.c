/*
 * A021937  Decimal expansion of 1/933.
 * https://oeis.org/A021937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit of the decimal expansion of 1/933 after the decimal
 *            point (a(0) is the first digit after the decimal point). The
 *            function uses long division; the remainder is kept in an
 *            unsigned integer large enough to hold the intermediate product.
 *            Digits are cached so that any order of calls is ef...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dynamic cache of computed digits */
    static seq_t *digits   = NULL;   /* array of digits */
    static size_t len     = 0;       /* number of digits stored */
    static size_t cap     = 0;       /* allocated capacity */
    static unsigned int rem = 1;     /* current remainder (1 initially) */

    /* initialise storage if first call */
    if (digits == NULL) {
        cap = 1;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!digits) return 0; /* allocation failed – unlikely in tests */
    }

    /* ensure capacity covers the requested index */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        digits = tmp;
        cap = new_cap;
    }

    /* compute digits up to n if necessary */
    while (len <= (size_t)n) {
        /* long‐division step:  (rem*10)/933 gives next digit */
        unsigned int prod  = rem * 10U;          /* rem < 933, prod < 9330 */
        seq_t digit = prod / 933U;               /* 0 – 9 */
        rem = prod % 933U;                       /* new remainder for next step */
        digits[len++] = digit;
    }

    return digits[(size_t)n];
}
