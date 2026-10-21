/*
 * A085125  Even numbers which are 7-smooth.
 * https://oeis.org/A085125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A002473(n) (even 7‑smooth numbers)
 * Range:     u8 n <= 52, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    static seq_t *hum = NULL;            /* array of 7‑smooth numbers */
    static size_t cap = 0;               /* allocated size */
    static idx_t  len = 0;               /* number of generated terms */
    static idx_t  i2 = 0, i3 = 0, i5 = 0, i7 = 0;   /* pointers for next multiples */

    /* Ensure capacity for at least n terms */
    if ((size_t)n > cap) {
        size_t newcap = (size_t)n > 2 * cap ? (size_t)n : 2 * cap;
        if (newcap == 0) newcap = 1;
        seq_t *tmp = (seq_t*)realloc(hum, newcap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        hum = tmp;
        cap = newcap;
    }

    /* Initialize first term if needed */
    if (len == 0) {
        hum[0] = 1;                      /* 7‑smooth number 1 */
        len = 1;
    }

    /* Generate additional terms up to n */
    for (idx_t k = len; k < n; ++k) {
        seq_t c2 = hum[i2] * (seq_t)2;
        seq_t c3 = hum[i3] * (seq_t)3;
        seq_t c5 = hum[i5] * (seq_t)5;
        seq_t c7 = hum[i7] * (seq_t)7;

        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        hum[k] = next;
        if (c2 == next) ++i2;
        if (c3 == next) ++i3;
        if (c5 == next) ++i5;
        if (c7 == next) ++i7;
    }
    len = n;

    return hum[n - 1] * (seq_t)2;        /* each term multiplied by 2 */
}
