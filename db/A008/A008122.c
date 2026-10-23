/*
 * A008122  Coordination sequence T2 for Zeolite Code JBW.
 * https://oeis.org/A008122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) – 2·a(n-2) + 2·a(n-3) – a(n-4) + 3·a(n-5) –
 *            2·a(n-6) + 4·a(n-7) – 4·a(n-8) + 2·a(n-9) – 3·a(n-10) + a(n-11)
 *            – 2·a(n-12) + 2·a(n-13) – a(n-14) + a(n-15) (n ≥ 16) Initial
 *            values (a(0)…a(15)) are taken from the OEIS entry
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values a(0) … a(15) */
    static const seq_t base_vals[16] = {
        (seq_t)1,   /* a(0) */
        (seq_t)4,   /* a(1) */
        (seq_t)12,  /* a(2) */
        (seq_t)24,  /* a(3) */
        (seq_t)36,  /* a(4) */
        (seq_t)56,  /* a(5) */
        (seq_t)86,  /* a(6) */
        (seq_t)118, /* a(7) */
        (seq_t)146, /* a(8) */
        (seq_t)176, /* a(9) */
        (seq_t)228, /* a(10) */
        (seq_t)286, /* a(11) */
        (seq_t)330, /* a(12) */
        (seq_t)376, /* a(13) */
        (seq_t)438, /* a(14) */
        (seq_t)518  /* a(15) */
    };

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static idx_t  max_index = 0;

    /* Initialise cache with the first 16 terms. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* fallback */
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = base_vals[i];
        max_index = 15;
    }

    /* If the requested index is already in the cache, return it. */
    if (n <= max_index)
        return cache[n];

    /* Extend the cache up to the requested index using the recurrence. */
    size_t new_len = (size_t)n + 1;        /* include index 0 … n */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;     /* fallback on allocation failure */
    cache = tmp;

    /* Compute new values. */
    for (idx_t i = max_index + 1; i <= n; ++i)
    {
        /* All indices i-k are valid because i ≥ 16. */
        seq_t val = cache[i-1];
        val -= (seq_t)2 * cache[i-2];
        val += (seq_t)2 * cache[i-3];
        val -= cache[i-4];
        val += (seq_t)3 * cache[i-5];
        val -= (seq_t)2 * cache[i-6];
        val += (seq_t)4 * cache[i-7];
        val -= (seq_t)4 * cache[i-8];
        val += (seq_t)2 * cache[i-9];
        val -= (seq_t)3 * cache[i-10];
        val += cache[i-11];
        val -= (seq_t)2 * cache[i-12];
        val += (seq_t)2 * cache[i-13];
        val -= cache[i-14];
        val += cache[i-15];
        cache[i] = val;
    }

    max_index = n;
    return cache[n];
}
