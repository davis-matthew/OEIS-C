/*
 * A137304  Decimal expansion of 999999999/9801.
 * https://oeis.org/A137304
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 104, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A137304:
 *  a(n) = the nth digit of the decimal expansion of 999999999/9801
 *  (offset 6, so a(6) is the first digit of the integer part 102030).
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* return value for n < 6 is undefined behaviour in the
     * original sequence; we simply return 0 here if requested.
     */
    if (n < 6) {
        return 0;
    }

    /* static state */
    static seq_t *digits = NULL;        /* array of computed digits */
    static idx_t  sz      = 0;          /* number of digits stored */
    static idx_t  cap     = 0;          /* current capacity of array */
    static seq_t  next_rem = 0;         /* remainder after integer part */
    static bool   rem_init = false;     /* has remainder been initialized? */

    /* initialisation (only once) */
    if (!digits) {
        /* capacity for 64 digits, will grow as needed */
        cap = 64;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!digits) {
            /* allocation failure – return 0 as a fallback */
            return 0;
        }

        /* integer part of the fraction */
        const seq_t num   = 999999999;
        const seq_t den   = 9801;
        const seq_t q     = num / den;          /* integer part = 102030 */
        const seq_t rem0  = num % den;          /* remainder = 3969 */

        /* precompute powers of ten for extracting integer digits */
        const seq_t pow10[6] = {
            (seq_t)100000,
            (seq_t)10000,
            (seq_t)1000,
            (seq_t)100,
            (seq_t)10,
            (seq_t)1
        };

        /* store first six digits (most‑significant to least‑significant) */
        for (idx_t i = 0; i < 6; ++i) {
            digits[i] = (q / pow10[i]) % 10;
        }
        sz = 6;

        /* save the remainder for generating decimal digits later */
        next_rem = rem0;
        rem_init = true;
    }

    /* desired number of digits: positions 6..n inclusive */
    idx_t need = n - 6 + 1;              /* length required */

    /* grow the array if necessary */
    while (sz < need) {
        /* generate next decimal digit */
        next_rem *= 10;
        seq_t digit = next_rem / 9801;
        next_rem   = next_rem % 9801;

        /* ensure capacity */
        if (sz >= cap) {
            cap = (cap > 0) ? cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(digits, cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure – return 0 as a fallback */
                return 0;
            }
            digits = tmp;
        }

        digits[sz++] = digit;
    }

    return digits[need - 1];
}
