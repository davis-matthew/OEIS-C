/*
 * A021519  Decimal expansion of 1/515.
 * https://oeis.org/A021519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   * a(n) = a(n‑1) - a(n‑17) + a(n‑18) for n > 18 * a(0) = 0, a(1)
 *            = 0, a(2) = 1, a(3) = 9, * a(4) = 4, a(5) = 1, a(6) = 7, a(7) =
 *            4, * a(8) = 7, a(9) = 5, a(10) = 7, a(11) = 2, * a(12) = 8,
 *            a(13) = 1, a(14) = 5, a(15) = 5, * a(16) = 3, a(17) = 3, a(18) =
 *            9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of A021519.  The function is idempotent and
 * memoises the computed values so that calls with arbitrary n occur
 * in any order. */
seq_t a(idx_t n)
{
    /* static cache array */
    static seq_t *cache     = NULL;  /* pointer to stored terms */
    static size_t cache_cap = 0;     /* allocated capacity (in terms) */
    static size_t cache_sz  = 0;     /* number of valid entries in cache */

    /* first‑time initialisation of the cache with the first 19 terms */
    if (cache_sz == 0) {
        /* three literal values of seq_t; explicit cast keeps the
         * code valid for arbitrary‑precision seq_t types.           */
        static const seq_t init_vals[19] = {
            (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)9, (seq_t)4,
            (seq_t)1, (seq_t)7, (seq_t)4, (seq_t)7, (seq_t)5,
            (seq_t)7, (seq_t)2, (seq_t)8, (seq_t)1, (seq_t)5,
            (seq_t)5, (seq_t)3, (seq_t)3, (seq_t)9
        };

        /* small initial capacity; will grow lazily as needed. */
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);

        /* copy the constants in the cache */
        for (size_t i = 0; i < 19; ++i)
            cache[i] = init_vals[i];
        cache_sz = 19;
    }

    /* fast path for indices that we have already finished. */
    if ((size_t)n < cache_sz)
        return cache[(size_t)n];

    /* grow the cache until it contains the requested index. */
    while (cache_sz <= (size_t)n) {
        /* grow the underlying storage if the capacity is exceeded. */
        if (cache_sz >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the next term via the recurrence.  We perform the
         * arithmetic in 32‑bit signed integers because all sequence
         * values are known to be single decimal digits (0…9).  The cast
         * from seq_t to int is safe for the supported integer
         * types and is provided for arbitrary‑precision types by
         * usual conversion operators.                          */
        int next = (int)cache[cache_sz - 1]
                 - (int)cache[cache_sz - 17]
                 + (int)cache[cache_sz - 18];

        cache[cache_sz] = (seq_t)next;
        ++cache_sz;
    }

    return cache[(size_t)n];
}
