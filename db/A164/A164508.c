/*
 * A164508  Number of binary strings of length n with no substrings equal to
 *          0101, 0110, or 1001.
 * https://oeis.org/A164508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A164508:
 * a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=13, a(5)=20, a(6)=33;
 * for n ≥ 7: a(n) = 2*a(n-1) – a(n-2) + 2*a(n-4) – 2*a(n-5) + a(n-6). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for 0 ≤ n ≤ 6 */
    static const seq_t base[7] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)8,
        (seq_t)13,
        (seq_t)20,
        (seq_t)33
    };

    if (n <= 6) return base[n];

    /* Static cache of computed values */
    static seq_t *cache = NULL;       /* pointer to allocated array */
    static idx_t cache_size = 0;      /* allocated size (number of elements) */
    static idx_t cache_last = 0;      /* highest index already computed */

    /* Initialise the cache on first call */
    if (cache == NULL) {
        cache_size = (idx_t)(n + 1);
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory – unlikely in tests */

        /* Fill known base values */
        for (idx_t i = 0; i <= 6; ++i) cache[i] = base[i];
        cache_last = 6;
    }

    /* Ensure the cache is large enough for index n */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Compute missing terms from cache_last+1 up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)2;
        val -= cache[i - 2];
        val += cache[i - 4] * (seq_t)2;
        val -= cache[i - 5] * (seq_t)2;
        val += cache[i - 6];
        cache[i] = val;
    }
    cache_last = n;

    return cache[n];
}
