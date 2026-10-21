/*
 * A070870  a(1) = 6; a(n+1) = (a(n)+1)/2 if a(n) odd, or 5*a(n)/2 if a(n)
 *          even.
 * https://oeis.org/A070870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(1) = 6 a(n+1) = (a(n)+1)/2 if a(n) odd = 5*a(n)/2 if a(n) even
 *            (computes without intermediate overflow for all values that fit
 *            in seq_t)
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 61, u64 n <= 170,
 *            u128 n <= 447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Simple memoisation of computed terms. */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cached     = 0;      /* number of terms stored           */
    static size_t capacity   = 0;      /* current allocation size          */

    if (n < 1) return (seq_t)0;        /* undefined for n == 0 */

    /* Return already cached value if it exists. */
    if (cached && n <= cached)
        return cache[n - 1];

    /* Grow allocation if needed. */
    size_t needed = (size_t)n;
    if (capacity < needed) {
        size_t newcap = capacity ? capacity * 2 : 8;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failure – return 0 */
        cache    = tmp;
        capacity = newcap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cached; i < needed; ++i) {
        if (i == 0) {                  /* a(1) base case */
            cache[i] = (seq_t)6;
        } else {
            seq_t prev = cache[i - 1];
            seq_t next;
            if ((prev & (seq_t)1) == 0)            /* even */
                next = (prev / 2) * 5;             /* safer to avoid overflow */
            else                                   /* odd */
                next = (prev + 1) / 2;
            cache[i] = next;
        }
    }

    cached = needed;
    return cache[n - 1];
}
