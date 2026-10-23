/*
 * A021213  Decimal expansion of 1/209.
 * https://oeis.org/A021213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n‑th digit of the decimal expansion of 1/209 (with a(0)
 *            being the first digit after the decimal point). The sequence
 *            satisfies the recurrence a(n) = a(n-1) - a(n-9) + a(n-10) for n
 *            > 9 with initial values a(0)=0, a(1)=0, a(2)=4, a(3)=7, a(4)=8,
 *            a(5)=4, a(6)=6, a(7)=8, a(8)=8, a(9)=9
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
    static seq_t *digits = NULL;   /* cached digits */
    static size_t cap    = 0;      /* allocated capacity */
    static size_t used   = 0;      /* number of digits stored (next free index) */
    static uint64_t rem   = 1;     /* remainder after last computed digit */

    /* Ensure we have enough storage for n+1 digits. */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = cap ? cap * 2 : 16;
        if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        digits = tmp;
        cap = new_cap;
    }

    /* Generate missing digits using long division. */
    for (size_t i = used; i <= (size_t)n; ++i)
    {
        rem *= 10ULL;
        seq_t d = (seq_t)(rem / 209ULL);
        rem = rem % 209ULL;
        digits[i] = d;
    }

    used = (size_t)n + 1;
    return digits[(size_t)n];
}
