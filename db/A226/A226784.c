/*
 * A226784  If gcd(n,6) != 1 then a(n)=0, otherwise a(n)=6^(-1) in Z/nZ*.
 * https://oeis.org/A226784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, if gcd(n,6) ≠ 1; otherwise a(n) = 6^(-1) modulo n. The
 *            sequence satisfies the recurrence a(n) = 2*a(n-6) - a(n-12) (n ≥
 *            14) with initial values a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1,
 *            a(6)=0, a(7)=6, a(8)=0, a(9)=0, a(10)=0, a(11)=2, a(12)=0,
 *            a(13)=11
 * Range:     u8 n <= 306, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);   /* declared elsewhere in the build */

seq_t a(idx_t n)
{
    /* Handle zero index (outside the official offset). */
    if (n == 0) return (seq_t)0;

    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;      /* array indexed from 1 */
    static idx_t cache_size = 0;     /* allocated size */
    static idx_t computed = 0;       /* highest index computed so far */

    /* Base values for 1 .. 13. */
    static const seq_t base[14] = {
        0,          /* index 0 placeholder */
        0, 0, 0, 0, 1, 0, 6, 0, 0, 0, 2, 0, 11
    };
    const idx_t base_count = 13;

    /* First call: allocate cache with an initial size. */
    if (cache == NULL) {
        /* Choose a modest power of two that covers the initial base + padding. */
        cache_size = 32;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* Out of memory; return zero. */
        memset(cache, 0, cache_size * sizeof(seq_t));

        /* Copy the known base values. */
        for (idx_t i = 1; i <= base_count; ++i) {
            cache[i] = base[i];
        }
        computed = base_count;
    }

    /* Grow the cache if the requested index exceeds its capacity. */
    if (n >= cache_size) {
        idx_t newsize = cache_size;
        while (newsize <= n) newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* Allocation failed */
        /* Zero the newly allocated portion. */
        memset(tmp + cache_size, 0, (newsize - cache_size) * sizeof(seq_t));
        cache = tmp;
        cache_size = newsize;
    }

    /* Compute any missing terms up to n incrementally. */
    while (computed < n) {
        ++computed;
        seq_t v6  = cache[computed - 6];
        seq_t v12 = cache[computed - 12];
        seq_t val = v6 + v6;   /* 2 * v6 */
        /* The recurrence should never produce a negative result for valid n. */
        if (val >= v12)
            cache[computed] = val - v12;
        else
            cache[computed] = (seq_t)0;  /* defensive fallback */
    }

    return cache[n];
}
