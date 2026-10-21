/*
 * A211866  a(n) = (9^n - 5) / 4.
 * https://oeis.org/A211866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^n - 5) / 4 a(1) = 1 recurrence: a(n) = 9*a(n-1) + 10
 *            for n > 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* grow cache to hold indices up to n (1‑based).  New elements are
   zero‑initialised, which guarantees that cache[i] == 0 means “not
   yet computed”. */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;           /* index 0 unused */
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                /* allocation failure – fatal in this
                               simple implementation */
        exit(1);
    cache = tmp;
    /* initialise new entries to zero.  Old elements already valid. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* retrieve a(n) for n >= 1.  The function works correctly for any
 * integer type seq_t supported by the build, including arbitrary‑precision
 * integers.
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    ensure_capacity(n);

    /* initialise the first term if not already done */
    if (cache[1] == 0)
        cache[1] = 1;      /* a(1) = 1 */

    /* compute missing terms up to n, using the recurrence a(k)=9*a(k-1)+10 */
    for (size_t k = 2; k <= (size_t)n; ++k)
        if (cache[k] == 0)
            cache[k] = cache[k - 1] * 9 + 10;

    return cache[(size_t)n];
}
