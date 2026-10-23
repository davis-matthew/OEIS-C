/*
 * A042088  Numerators of continued fraction convergents to sqrt(568).
 * https://oeis.org/A042088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 286*a(n-4) - a(n-8) for n >= 8 Initial values: a(0)=23,
 *            a(1)=24, a(2)=119, a(3)=143, a(4)=6697, a(5)=6840, a(6)=34057,
 *            a(7)=40897
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed values */
static idx_t  capacity = 0;      /* allocated size */
static idx_t  len      = 0;      /* number of values actually computed */

static void grow_cache(idx_t n)
{
    /* Ensure we have at least the first 8 values pre‑filled */
    if (len == 0) {
        capacity = 8;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();  /* allocation failed */

        /* initialise a(0) … a(7) */
        cache[0] = (seq_t)23;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)119;
        cache[3] = (seq_t)143;
        cache[4] = (seq_t)6697;
        cache[5] = (seq_t)6840;
        cache[6] = (seq_t)34057;
        cache[7] = (seq_t)40897;

        len = 8;
    }

    if (n < len)
        return;          /* already computed */

    /* enlarge capacity if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* compute new entries */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence requires i-4 and i-8 to exist, which is true for i>=8 */
        cache[i] = (286 * cache[i - 4]) - cache[i - 8];
    }
    len = n + 1;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[n];
}
