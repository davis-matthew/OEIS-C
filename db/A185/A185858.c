/*
 * A185858  1/128 the number of (n+2) X 3 binary arrays with no 3 X 3 subblock
 *          trace equal to any horizontal or vertical neighbor 3 X 3 subblock
 *          trace.
 * https://oeis.org/A185858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 9*a(n-2) for n>=3 Initial values: a(1)=4,
 *            a(2)=22 Recurrence yields the OEIS sequence A185858
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

seq_t a(idx_t n)
{
    if (n < 1U)          /* the sequence is defined for n>=1 */
        return (seq_t)0;

    static seq_t *cache = NULL;  /* 0‑based: cache[0] = a(1) */
    static size_t len   = 0;     /* current number of stored terms */
    static size_t cap   = 0;     /* allocated capacity */

    /* index in cache: a(n) -> cache[n-1] */
    size_t idx = (size_t)(n - 1U);

    /* Grow cache until it can hold a(n) */
    if (len <= idx) {
        size_t need = idx + 1U;
        if (cap < need) {
            size_t newcap = cap ? cap * 2U : 1U;
            if (newcap < need) newcap = need;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failed */
            cache = tmp;
            cap = newcap;
        }

        /* Initialise first two terms if cache is empty */
        if (len == 0U) {
            cache[0] = (seq_t)4;      /* a(1) */
            len = 1U;
        }
        if (len == 1U && need > 1U) {
            cache[1] = (seq_t)22;     /* a(2) */
            len = 2U;
        }

        /* Compute further terms up to 'need' */
        for (size_t i = len; i < need; ++i) {
            /* 4*a(i) + 9*a(i-1) where indices are 0‑based */
            cache[i] = cache[i-1] * (seq_t)4 + cache[i-2] * (seq_t)9;
        }
        if (len < need) len = need;
    }

    return cache[idx];
}
