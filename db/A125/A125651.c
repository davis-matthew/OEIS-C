/*
 * A125651  Numbers k such that A125650(k) is a perfect square.
 * https://oeis.org/A125651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=24; For n>1: a(n+2) = 6*a(n+1) - a(n) + 6
 *            (recurrence from OEIS)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, safety */

    /* static cache of already computed terms (1‑based index) */
    static seq_t *val = NULL;
    static idx_t  cap  = 0;               /* size of allocated array (1‑based) */
    static idx_t  max  = 0;               /* highest index computed so far */
    static bool   init = false;          /* whether base terms are initialised */

    /* grow the array if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;             /* +1 to allow index 'n' directly */
        seq_t *tmp = (seq_t *)realloc(val, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        val = tmp;
        cap = new_cap;
    }

    /* initialise base values once */
    if (!init) {
        val[1] = (seq_t)1;
        if (cap > 2) val[2] = (seq_t)3;
        if (cap > 3) val[3] = (seq_t)24;
        init = true;
        /* mark the highest computed index after initialization */
        max = (cap > 3) ? 3 : (cap > 2 ? 2 : 1);
    } else {
        /* after a reallocation we need to make sure base terms exist */
        if (max < 3) {
            val[1] = (seq_t)1;
            if (cap > 2) val[2] = (seq_t)3;
            if (cap > 3) val[3] = (seq_t)24;
            max = (cap > 3) ? 3 : (cap > 2 ? 2 : 1);
        }
    }

    /* compute forward using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t t = 6 * val[i - 1];   /* 6 * a(i-1) */
        t -= val[i - 2];           /* minus a(i-2) */
        val[i] = t + (seq_t)6;     /* plus 6 */
    }
    max = n;  /* update highest computed index */

    return val[n];
}
