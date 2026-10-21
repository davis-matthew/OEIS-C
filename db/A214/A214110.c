/*
 * A214110  Number of 0..3 colorings on an nX7 array circular in the 7
 *          direction with new values 0..3 introduced in row major order.
 * https://oeis.org/A214110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 416*a(n-1) -15454*a(n-2) +89758*a(n-3) +90848*a(n-4)
   -438718*a(n-5) +62801*a(n-6)      (n ≥ 7)
   Initial values: a(1)=91, a(2)=34041, a(3)=12777541, a(4)=4797577911,
   a(5)=1801391900581, a(6)=676387200779521 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* undefined behaviour for n < 1: return 0 (offset 1) */
    if (n < 1) return 0;

    /* for the first six terms, use the constants directly */
    if (n <= 6) {
        /* static list of the six initial terms */
        static const seq_t init[6] = {
            91ULL,                 /* a(1) */
            34041ULL,              /* a(2) */
            12777541ULL,           /* a(3) */
            4797577911ULL,         /* a(4) */
            1801391900581ULL,      /* a(5) */
            676387200779521ULL     /* a(6) */
        };
        return init[n-1];
    }

    /* Memoised cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;   /* number of stored terms */
    static size_t cap   = 0;   /* capacity of the allocated array */

    /* Static list of initial terms (will be copied into cache) */
    static const seq_t init[6] = {
        91ULL,
        34041ULL,
        12777541ULL,
        4797577911ULL,
        1801391900581ULL,
        676387200779521ULL
    };

    /* Initialise cache if necessary */
    if (len == 0) {
        /* Start with at least 64 slots, or enough for the requested n */
        cap = (size_t)n > 64 ? (size_t)n : 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – unlikely */
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        len = 6;
    }
    /* Grow the cache if we need to store more terms */
    else if (len < n) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failure – unlikely */
            cache = tmp;
            cap = new_cap;
        }
    }

    /* Compute missing terms up to n using the recurrence */
    while (len < n) {
        /* index of the new term is len (1‑based: a(len+1)) */
        seq_t v1 = cache[len-1]; /* a(len)   */
        seq_t v2 = cache[len-2]; /* a(len-1) */
        seq_t v3 = cache[len-3]; /* a(len-2) */
        seq_t v4 = cache[len-4]; /* a(len-3) */
        seq_t v5 = cache[len-5]; /* a(len-4) */
        seq_t v6 = cache[len-6]; /* a(len-5) */

        seq_t val = 0;
        val += 416   * v1;
        val -= 15454 * v2;
        val += 89758 * v3;
        val += 90848 * v4;
        val -= 438718 * v5;
        val += 62801 * v6;

        cache[len] = val;
        ++len;
    }

    return cache[n-1];
}
