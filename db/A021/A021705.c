/*
 * A021705  Decimal expansion of 1/701.
 * https://oeis.org/A021705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/701: a(n) = digit at position n after the decimal point */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed digits */
    static seq_t *cache = NULL;   /* array of seq_t digits */
    static size_t len  = 0;       /* number of digits stored in cache */
    static size_t cap  = 0;       /* allocated capacity of cache  */
    static uint64_t rem = 1;      /* remainder after the last computed digit   */

    /* Convert index to size_t for array indexing.  The sequence is
       only used for n up to 10 000 in the tests, so a truncation
       to size_t is safe. */
    size_t idx = (size_t)n;

    /* Return cached digit if already computed */
    if (idx < len)
        return cache[idx];

    /* Grow cache if needed */
    if (idx >= cap) {
        size_t newcap = cap ? cap * 2 : 8;
        while (newcap <= idx)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)          /* allocation failed – return zero */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing digits from len up to idx (inclusive). */
    for (size_t i = len; i <= idx; ++i) {
        rem *= 10ULL;                     /* bring next decimal place into remainder   */
        uint64_t d  = rem / 701ULL;        /* next digit  (0–9)                         */
        rem        %= 701ULL;              /* new remainder after extracting the digit  */
        cache[i]   = (seq_t)d;            /* store digit in the cache                 */
    }
    len = idx + 1;                         /* update number of stored digits            */

    return cache[idx];
}
