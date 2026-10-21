/*
 * A207025  Number of 2 X n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 34, u64 n <= 74, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207025:
 * a(n) = 2*a(n-1) + a(n-2) - a(n-3) - 4*a(n-4) + 2*a(n-5)
 *        + a(n-7) + a(n-9) - a(n-10)
 * initial values: a(1)=4, a(2)=16, a(3)=36, a(4)=81, a(5)=169,
 *                 a(6)=324, a(7)=625, a(8)=1156, a(9)=2116, a(10)=3844
 */

#include <stdlib.h>

static const seq_t init[10] = {4,16,36,81,169,324,625,1156,2116,3844};

seq_t a(idx_t n)
{
    /* on the first call static variables are zero-initialised */
    static seq_t *cache  = NULL;   /* array of computed terms (1‑based stored in 0‑based index) */
    static size_t cap   = 0;        /* capacity of the array in elements */
    static size_t sz    = 0;        /* number of values actually stored */

    /* ensure the array has room for n elements (1‑based)   */
    if ((size_t)n > cap) {
        size_t newcap = cap ? (cap << 1) : 16;
        if (newcap < (size_t)n) newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);            /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute terms up to n (zero‑based indices i = 0 … n‑1)      */
    for (size_t i = sz; i < (size_t)n; ++i) {
        if (i < 10)          /* use the known initial values          */
            cache[i] = init[i];
        else {               /* recurrence for n = i+1 (index i ≥ 10) */
            seq_t t = 0;
            t += 2 * cache[i-1];          /* 2*a(n-1) */
            t += cache[i-2];              /* +a(n-2) */
            t -= cache[i-3];              /* -a(n-3) */
            t -= (seq_t)4 * cache[i-4];    /* -4*a(n-4) */
            t += 2 * cache[i-5];          /* +2*a(n-5) */
            t += cache[i-7];              /* +a(n-7) */
            t += cache[i-9];              /* +a(n-9) */
            t -= cache[i-10];             /* -a(n-10) */
            cache[i] = t;
        }
    }
    sz = (size_t)n;          /* update number of stored terms */

    return cache[n-1];
}
