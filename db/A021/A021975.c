/*
 * A021975  Decimal expansion of 1/971.
 * https://oeis.org/A021975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/971 (a(0) is the first digit
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits and current remainder */
    static seq_t *digits = NULL;      /* array of computed decimal digits */
    static size_t cap = 0;            /* allocated capacity of the array */
    static idx_t len = 0;             /* number of digits stored (len > 0 after first use) */
    static uint64_t rem = 1;          /* current remainder (always < 971) */

    /* initialise cache on first call */
    if (digits == NULL) {
        cap = 16;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        len  = 0;          /* no digits stored yet */
        rem = 1;           /* start with numerator 1 */
    }

    /* compute digits until we have at least n+1 digits */
    while (len <= n) {
        /* generate next decimal digit of 1/971 */
        rem *= 10;                         /* bring down next zero */
        seq_t digit = (seq_t)(rem / 971);   /* quotient is the digit (0-9) */
        rem %= 971;                        /* new remainder for next step */

        /* grow cache if needed */
        if ((size_t)len >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 16;
            while (new_cap <= (size_t)n + 1) new_cap *= 2;
            seq_t *new_ptr = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (new_ptr == NULL) abort();  /* allocation failure: terminate */
            digits = new_ptr;
            cap = new_cap;
        }

        digits[len++] = digit;               /* store the new digit */
    }

    return digits[(size_t)n];
}
