/*
 * A021858  Decimal expansion of 1/854.
 * https://oeis.org/A021858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 1/854.  a(0) is the first digit after the decimal point. */
#include <stdlib.h>

static idx_t rem = 1;                  /* current remainder of division 1/854 */
static size_t cap = 0;                 /* allocated capacity of the digit cache */
static size_t size = 0;                /* number of digits currently cached */
static seq_t *digits = NULL;           /* cache of computed digits */

static void ensure_cap(size_t need)
{
    if (need <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    digits = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Allocate cache if needed */
    if (digits == NULL)
        ensure_cap(16);

    /* Generate digits up to n (inclusive) */
    while (size <= (size_t)n) {
        idx_t tmp = rem * (idx_t)10;            /* remainder * 10 */
        seq_t digit = (seq_t)(tmp / 854);       /* next decimal digit */
        rem = tmp % 854;                        /* update remainder */

        ensure_cap(size + 1);
        digits[size++] = digit;
    }

    return digits[(size_t)n];
}
