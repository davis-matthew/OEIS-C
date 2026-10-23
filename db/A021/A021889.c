/*
 * A021889  Decimal expansion of 1/885.
 * https://oeis.org/A021889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point in the decimal
 *            expansion of 1/885
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *digits = NULL;       /* array of digits (0-9) */
    static idx_t  len     = 0;         /* current number of computed digits */
    static idx_t  cap     = 0;         /* current allocation size */
    static uint64_t rem   = 1;         /* current remainder (0 <= rem < 885) */

    /* grow the cache if necessary */
    if (n + 1 > len) {
        /* allocate or grow the array to at least n+1 elements */
        if (n + 1 > cap) {
            idx_t new_cap = cap ? cap << 1 : 16;
            if (new_cap < n + 1) new_cap = n + 1;
            seq_t *new_d = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!new_d) return 0; /* OOM: return 0 as a fallback */
            digits = new_d;
            cap = new_cap;
        }

        /* compute missing digits up to index n */
        for (idx_t i = len; i <= n; ++i) {
            rem = rem * 10ULL;            /* multiply remainder by 10 */
            seq_t d = (seq_t)(rem / 885ULL); /* next digit */
            rem = rem % 885ULL;          /* update remainder */
            digits[i] = d;
        }
        len = n + 1;
    }

    return digits[n];
}
