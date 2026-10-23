/*
 * A021752  Decimal expansion of 1/748.
 * https://oeis.org/A021752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/748 (n ≥ 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function lazily computes the digits; results are cached for later calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_computed = (idx_t)-1;  /* no digits computed yet */
    static uint64_t rem = 1;                /* remainder after the last computed digit */

    /* ensure capacity */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t newcap = (cap == 0 ? 16 : cap * 2);
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);          /* insufficient memory – abort */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing digits */
    if (max_computed == (idx_t)-1) {    /* first call */
        rem = 1;
        max_computed = (idx_t)-1;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        rem *= 10ULL;
        seq_t digit = (seq_t)(rem / 748ULL);
        rem %= 748ULL;
        cache[(size_t)i] = digit;
        max_computed = i;
    }

    return cache[(size_t)n];
}
