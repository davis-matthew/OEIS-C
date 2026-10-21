/*
 * A238540  A fourth-order linear divisibility sequence: a(n) := (3^n +
 *          1)*(3^(3*n) - 1)/( (3 + 1)*(3^3 - 1)).
 * https://oeis.org/A238540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 1)*(3^(3*n) - 1)/104 ; Recurrence: a(n) =
 *            112*a(n-1) - 2622*a(n-2) + 9072*a(n-3) - 6561*a(n-4); Initial
 *            values: a(1)=1, a(2)=70, a(3)=5299, a(4)=419020
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of A238540.  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[0] = a(1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored in cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialise cache with the first four terms on the first call. */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out of memory */
        cache[0] = (seq_t)1;         /* a(1) */
        cache[1] = (seq_t)70;        /* a(2) */
        cache[2] = (seq_t)5299;      /* a(3) */
        cache[3] = (seq_t)419020;    /* a(4) */
        cache_len = 4;
    }

    /* If n is already computed, return it. */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];          /* 1‑based to 0‑based */

    /* Extend cache up to n. */
    while (cache_len < n) {
        /* Ensure enough capacity. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)(n + 1))
                new_cap = (size_t)(n + 1);
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;  /* out of memory */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Compute next term using the 4‑term recurrence. */
        seq_t a1 = cache[cache_len - 1];   /* a(n-1) */
        seq_t a2 = cache[cache_len - 2];   /* a(n-2) */
        seq_t a3 = cache[cache_len - 3];   /* a(n-3) */
        seq_t a4 = cache[cache_len - 4];   /* a(n-4) */

        seq_t next = 112 * a1;
        next -= 2622 * a2;
        next += 9072 * a3;
        next -= 6561 * a4;

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
