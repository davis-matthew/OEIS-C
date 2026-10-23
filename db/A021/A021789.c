/*
 * A021789  Decimal expansion of 1/785.
 * https://oeis.org/A021789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 1/785 (digits of 1/785 after the
 *            decimal point, 0‑based)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *digits = NULL;     /* array of digits */
    static size_t cap    = 0;        /* allocated capacity */
    static size_t len    = 0;        /* number of digits currently cached */
    static seq_t rem     = 1;        /* remainder after len digits (initially 1) */

    /* if we need more digits, compute them */
    if ((size_t)n >= len) {
        size_t needed = (size_t)n + 1;          /* we need digit n */
        if (needed > cap) {                    /* grow array if necessary */
            size_t newcap = cap ? cap * 2 : 16;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(digits, newcap * sizeof(seq_t));
            if (!tmp) return 0;                /* out‑of‑memory fallback */
            digits = tmp;
            cap = newcap;
        }

        /* compute missing digits using long division */
        for (; len <= (size_t)n; ++len) {
            rem *= 10;
            digits[len] = rem / 785;   /* integer division gives the digit */
            rem = rem % 785;           /* new remainder for next step */
        }
    }

    return digits[(size_t)n];
}
