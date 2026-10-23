/*
 * A037774  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0,2.
 * https://oeis.org/A037774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3 * For n > 1 a(n) = a(n‑1) * 6 + d_n * where d_n is the
 *            (n‑th) digit of the infinite periodic sequence * 3, 1, 0, 2, ...
 *            (pattern[0] = 3, pattern[1] = 1, pattern[2] = 0, pattern[3] =
 *            2). * Thus a(n) is the integer whose base‑6 representation
 *            consists of the * first n digits of that patter...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached a(1)…a(n) values       */
static size_t cacheSize = 0;     /* number of values currently cached */

static void
ensure_cache(idx_t n)
{
    if (n <= cacheSize)          /* already have a(1)…a(n)      */
        return;

    size_t needed = (size_t)n;
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    }

    const int pattern[4] = {3, 1, 0, 2};

    if (cacheSize == 0) {        /* initialise first term */
        cache[0] = 3;
        cacheSize = 1;
    }

    for (size_t i = cacheSize; i < needed; ++i) {
        /* digit for position i (0‑based) is pattern[i & 3]              */
        cache[i] = cache[i - 1] * (seq_t)6 + (seq_t)pattern[i & 3];
    }

    cacheSize = needed;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined below offset 1, safety value */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
