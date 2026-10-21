/*
 * A224603  Number of (n+2) X 7 0..2 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n > 8, with initial values a(1)=628, a(2)=630,
 *            a(3)=782, a(4)=906, a(5)=1202, a(6)=1609, a(7)=2091, a(8)=2817
 * Range:     u8 none, u16 n <= 16, u32 n <= 46, u64 n <= 104, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* first 8 terms of the sequence */
static const seq_t init8[8] = {
    (seq_t)628, (seq_t)630, (seq_t)782, (seq_t)906,
    (seq_t)1202, (seq_t)1609, (seq_t)2091, (seq_t)2817
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache: sequence values stored with 1‑based indexing */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity (number of slots) */
    static idx_t computed = 0;     /* highest index computed so far (1‑based) */

    /* Ensure sufficient capacity for index n */
    if ((idx_t)n > cap) {
        size_t new_cap = (size_t)n * 2;
        if (new_cap < 10) new_cap = 10;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;               /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise the first eight values if not already done */
    if (computed < 8) {
        memcpy(cache, init8, 8 * sizeof(seq_t));
        computed = 8;
    }

    if (n <= 8) return cache[n - 1];

    /* Compute terms up to n (if needed) */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* use 128‑bit intermediate to avoid overflow */
            unsigned __int128 sum = (unsigned __int128)cache[i - 2] * 4
                                + (unsigned __int128)cache[i - 4] * 5
                                + (unsigned __int128)cache[i - 6] * 3;
            unsigned __int128 sub = (unsigned __int128)cache[i - 3] * 6
                                + (unsigned __int128)cache[i - 5] * 4
                                + (unsigned __int128)cache[i - 7];
            cache[i - 1] = (seq_t)(sum - sub);
        }
        computed = n;
    }

    return cache[n - 1];
}
