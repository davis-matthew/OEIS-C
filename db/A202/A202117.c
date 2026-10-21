/*
 * A202117  Number of -1..1 arrays of n elements with first, second and third
 *          differences also in -1..1.
 * https://oeis.org/A202117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) for n > 8 Base
 *            values: a(1)=3, a(2)=7, a(3)=13, a(4)=19, a(5)=27, a(6)=35,
 *            a(7)=47, a(8)=65
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 55, u64 n <= 113,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t ready = 0;          /* largest n already computed */
    static seq_t *buf = NULL;        /* 1‑based cache of values */
    static size_t cap = 0;           /* allocated size (n+1) */

    /* Base terms (index 0 unused) */
    static const seq_t base[9] = {0, 3, 7, 13, 19, 27, 35, 47, 65};

    /* If value already computed, return it immediately */
    if (n <= ready && n < cap)
        return buf[n];

    /* Extend allocation if necessary */
    if (n >= cap) {
        size_t new_cap = n + 1;          /* 1‑based indexing */
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        buf = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = ready + 1; i <= n; ++i) {
        if (i <= 8)
            buf[i] = base[i];
        else
            buf[i] = 2 * buf[i-1] - buf[i-2] + buf[i-3] - buf[i-4];
    }
    ready = n;

    return buf[n];
}
