/*
 * A021809  Decimal expansion of 1/805.
 * https://oeis.org/A021809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in the decimal
 *            expansion of 1/805 The sequence starts with a(0)=0, a(1)=0,
 *            a(2)=1, …
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */
static seq_t *digits = NULL;    /* cached digits of the expansion            */
static size_t  len    = 0;      /* number of digits currently stored          */
static size_t  cap    = 0;      /* current capacity of the digits array       */
static uint64_t rem   = 1;      /* remainder before generating next digit     */

/* Return the nth digit (0‑based) of 1/805. */
seq_t a(idx_t n)
{
    /* Extend the cache until it contains the requested digit. */
    while (len <= (size_t)n) {
        /* Allocate more space if needed. */
        if (len >= cap) {
            size_t new_cap = cap ? (cap << 1) : 16;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Out‑of‑memory: abort. */
                exit(EXIT_FAILURE);
            }
            digits = tmp;
            cap    = new_cap;
        }

        /* One step of the long‑division algorithm. */
        uint64_t t   = rem * 10ULL;            /* remainder × 10          */
        seq_t    d   = (seq_t)(t / 805ULL);    /* next decimal digit      */
        rem          = t % 805ULL;             /* new remainder           */

        digits[len++] = d;                     /* store the digit         */
    }

    return digits[(size_t)n];
}
