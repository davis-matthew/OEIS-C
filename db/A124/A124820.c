/*
 * A124820  Expansion of (1-x)/(1-4*x+3*x^2-x^3).
 * https://oeis.org/A124820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) with initial values a(0)=1,
 *            a(1)=3, a(2)=9. The recurrence follows from the generating
 *            function (1‑x)/(1‑4x+3x²‑x³)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;      /* sequence cache */
    static size_t buflen = 0;      /* allocated size of buf   */
    static idx_t last = 0;         /* highest index computed  */

    /* first call: allocate cache and initialise seed values */
    if (buf == NULL) {
        size_t init = 3;                   /* at least 3 needed for a(0..2) */
        buf = (seq_t *)malloc(init * sizeof(seq_t));
        if (buf == NULL) return (seq_t)0;  /* out‑of‑memory fallback */
        buflen = init;
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)3;
        buf[2] = (seq_t)9;
        last   = 2;                        /* indices 0..2 computed */
    }

    /* if the requested element is already computed, return it */
    if (n <= last) {
        return buf[n];
    }

    /* grow the cache if necessary */
    if (buflen <= (size_t)n) {
        size_t newlen = buflen;
        while (newlen <= (size_t)n) {
            newlen = newlen * 2;
        }
        seq_t *tmp = (seq_t *)realloc(buf, newlen * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* out‑of‑memory fallback */
        buf    = tmp;
        buflen = newlen;
    }

    /* compute missing sequence values using the linear recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        buf[i] = (seq_t)4 * buf[i-1] - (seq_t)3 * buf[i-2] + buf[i-3];
    }
    last = n;          /* update the highest computed index */
    return buf[n];
}
