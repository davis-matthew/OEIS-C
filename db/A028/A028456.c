/*
 * A028456  Number of perfect matchings in graph P_{2} X C_{4} X P_{n}.
 * https://oeis.org/A028456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 24*a(n-1) + 27*a(n-2) - 455*a(n-3) + 455*a(n-4)
   - 27*a(n-5) - 24*a(n-6) + a(n-7)
   Initial values:
   a(0)=1, a(1)=9, a(2)=272, a(3)=6345,
   a(4)=155969, a(5)=3794880, a(6)=92524801 */
#include <stdlib.h> /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = (idx_t)-1; /* index of last computed term */

    /* Ensure capacity for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? (cap << 1) : 8;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize first seven terms if not done yet */
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)9, (seq_t)272,
        (seq_t)6345, (seq_t)155969, (seq_t)3794880,
        (seq_t)92524801
    };
    if (last == (idx_t)-1 || last < 6) {
        for (size_t i = 0; i < 7 && i < cap; ++i)
            cache[i] = init[i];
        last = 6;
    }

    if (n <= last)
        return cache[(size_t)n];

    /* Compute missing terms up to index n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 24 * cache[(size_t)(i - 1)] +
                    27 * cache[(size_t)(i - 2)] +
                    455 * cache[(size_t)(i - 4)] +
                    cache[(size_t)(i - 7)];
        seq_t sub = 455 * cache[(size_t)(i - 3)] +
                    27 * cache[(size_t)(i - 5)] +
                    24 * cache[(size_t)(i - 6)];
        cache[(size_t)i] = val - sub;
    }
    last = n;
    return cache[(size_t)n];
}
