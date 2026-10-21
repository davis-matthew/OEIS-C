/*
 * A215139  a(n) = (a(n-1) - a(n-3))*7^((1+(-1)^n)/2) with a(6)=5, a(7)=4,
 *          a(8)=22.
 * https://oeis.org/A215139
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)-a(n-3))*7^((1+(-1)^n)/2) * with a(6)=5, a(7)=4,
 *            a(8)=22; defined for n ≥ 6
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* memoization array */
static size_t  a_cap   = 0;      /* allocated capacity */
static idx_t   a_up_to = 5;      /* highest index computed so far (at least 5) */

static void ensure_capacity(idx_t n)
{
    if ((size_t)n >= a_cap) {
        size_t new_cap = a_cap ? a_cap : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 6)                 /* behaviour below offset is irrelevant */
        return 0;

    ensure_capacity(n);        /* ensure array large enough for index n */

    /* initialise base values (once) */
    if (a_up_to < 8) {
        /* base a(6)=5, a(7)=4, a(8)=22 */
        if (a_cap <= 8)        /* guard against very small initial capacity */
            ensure_capacity(8);
        a_cache[6] = (seq_t)5;
        a_cache[7] = (seq_t)4;
        a_cache[8] = (seq_t)22;
        a_up_to   = 8;
    }

    if (n <= 8)                /* return known base value */
        return a_cache[n];

    /* compute missing terms up to n (any order of calls is handled) */
    for (idx_t i = a_up_to + 1; i <= n; ++i) {
        seq_t diff = a_cache[i-1] - a_cache[i-3];
        if ((i & 1) == 0)      /* even n: exponent = 1 */
            a_cache[i] = diff * (seq_t)7;
        else                   /* odd n: exponent = 0, multiply by 1 */
            a_cache[i] = diff;
    }
    a_up_to = n;
    return a_cache[n];
}
