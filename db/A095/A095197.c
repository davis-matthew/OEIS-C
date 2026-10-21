/*
 * A095197  a(1) = 1, a(n+1) = 2 * digit reversal of a(n).
 * https://oeis.org/A095197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1,  a(n+1) = 2 * rev(a(n)) where rev reverses the
 * decimal digits of its argument.  The sequence grows more than
 * exponentially, but the numbers fit into seq_t (which may be an
 * arbitrary‑precision type).  We compute the terms iteratively, storing
 * them in a static cache so that repeated calls with any n are fast.   */

#include <stdlib.h>

/* Reverse the decimal digits of a seq_t value.  Works for any integer
 * type that supports % 10, / 10, + and * 10. */
static seq_t rev_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1 */

    /* static cache of computed terms */
    static seq_t *cache = NULL; /* holds a(1) ... a(len) */
    static size_t len   = 0;     /* number of computed terms */
    static size_t cap   = 0;     /* allocated capacity */

    /* initialise cache on first call */
    if (len == 0) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)
            return 0;           /* allocation failure */
        cache[0] = (seq_t)1;    /* a(1) */
        len = 1;
    }

    /* Grow cache if needed */
    if ((size_t)n > cap) {
        size_t newcap = cap * 2;
        while (newcap < (size_t)n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp)
            return 0;           /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* compute any missing terms up to n */
    for (size_t i = len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = rev_digits(prev);
        cache[i]   = rev * 2;
    }
    len = (size_t)n;

    return cache[n - 1];
}
