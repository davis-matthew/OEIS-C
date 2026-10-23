/*
 * A195659  Number of ways to place 10n nonattacking kings on a vertical
 *          cylinder 20 X 2n.
 * https://oeis.org/A195659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-11) - 40*a(n-10) + 181*a(n-9) - 489*a(n-8) + *
 *            876*a(n-7) - 1092*a(n-6) + 966*a(n-5) - 606*a(n-4) + *
 *            264*a(n-3) - 76*a(n-2) + 13*a(n-1) for n > 11 * with initial
 *            values * a(1)=2048, a(2)=148352, a(3)=3076180, a(4)=33175486, *
 *            a(5)=239238888, a(6)=1314160492, a(7)=5937279840, * a(8)=...
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 31, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* base values for n = 1 .. 11 */
    static const seq_t base[12] = {
        0ULL,                 /* base[0] unused */
        2048ULL,              /* a(1) */
        148352ULL,            /* a(2) */
        3076180ULL,           /* a(3) */
        33175486ULL,          /* a(4) */
        239238888ULL,         /* a(5) */
        1314160492ULL,        /* a(6) */
        5937279840ULL,        /* a(7) */
        23174548666ULL,       /* a(8) */
        80812754568ULL,       /* a(9) */
        257860425672ULL,      /* a(10) */
        766319864440ULL       /* a(11) */
    };

    /* handle small n directly */
    if (n <= 11u)
        return base[n];

    /* static cache of previously computed values */
    static seq_t *cache   = NULL; /* cache[1..cache_size] are valid */
    static idx_t cache_size = 0;  /* highest n stored */

    /* if we already have the desired value, return it */
    if (cache && n <= cache_size)
        return cache[n];

    /* need to compute up to n */
    idx_t new_size = n;
    size_t new_capacity = (size_t)new_size + 1;   /* +1 to use 1‑based indexing */

    /* allocate new cache */
    seq_t *new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    if (!new_cache)
        return 0;   /* out of memory – caller will handle the zero result */

    /* copy existing values if any */
    if (cache) {
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];
    }

    /* ensure base values are present (they are needed up to n) */
    for (idx_t i = 1; i <= 11u; ++i)
        new_cache[i] = base[i];

    /* compute missing terms using the recurrence */
    for (idx_t i = 12u; i <= new_size; ++i) {
        new_cache[i] =
            (seq_t) 4   * new_cache[i - 11] -
            (seq_t)40  * new_cache[i - 10] +
            (seq_t)181 * new_cache[i - 9]  -
            (seq_t)489 * new_cache[i - 8]  +
            (seq_t)876 * new_cache[i - 7]  -
            (seq_t)1092* new_cache[i - 6]  +
            (seq_t)966 * new_cache[i - 5]  -
            (seq_t)606 * new_cache[i - 4]  +
            (seq_t)264 * new_cache[i - 3]  -
            (seq_t)76  * new_cache[i - 2]  +
            (seq_t)13  * new_cache[i - 1];
    }

    /* free old cache and replace with the expanded one */
    if (cache)
        free(cache);
    cache = new_cache;
    cache_size = new_size;

    return cache[n];
}
