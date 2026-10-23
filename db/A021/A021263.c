/*
 * A021263  Decimal expansion of 1/259.
 * https://oeis.org/A021263
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

/* Sequence A021263: Decimal expansion of 1/259.
   a(n) is the n-th digit after the decimal point of 1/259. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Build the repeating block of digits of 1/259 once. */
    static unsigned char digits[259];
    static idx_t period_len = 0;
    static int ready = 0;            /* flag to avoid recomputation */

    if (!ready) {
        idx_t r = 1;                 /* current remainder */
        idx_t pos = 0;               /* position in digits array */
        idx_t seen[259];
        for (idx_t i = 0; i < 259; ++i) seen[i] = (idx_t)-1;

        /* Iteratively perform long division to generate digits
           until a remainder repeats, indicating the start of a cycle. */
        while (seen[r] == (idx_t)-1) {
            seen[r] = pos;
            unsigned int d = (unsigned int)(10 * r / 259);
            digits[pos] = (unsigned char)d;
            r = (10 * r) % 259;
            ++pos;
        }
        period_len = pos - seen[r];   /* length of repeating block */
        ready = 1;
    }

    idx_t idx = n % period_len;       /* index into the repeating cycle */
    return (seq_t)digits[idx];
}
