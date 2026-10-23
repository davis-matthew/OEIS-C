/*
 * A182899  Number of returns to the horizontal axis (both from above and
 *          below) in all weighted lattice paths in L_n.
 * https://oeis.org/A182899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} k*A182898(n,k) = 2*A182897(n) Recurrence
 *            (order 8): n*a(n) + (-4n+3)a(n-1) + (2n-3)a(n-2) + 11(n-3)a(n-4)
 *            + (2n-9)a(n-6) + (-4n+21)a(n-7) + (n-6)a(n-8) = 0 Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=6, a(5)=18,
 *            a(6)=54, a(7)=152 The function computes a(n) for any n >=...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Ensure cache has room for indices up to n inclusive */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > n)
        return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; abort to avoid undefined behaviour */
        abort();
    }
    /* Initialize new elements to 0 if any */
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

extern seq_t a(idx_t n)
{
    /* Initial sequence values */
    const idx_t init_vals[8] = {0,0,0,2,6,18,54,152};

    ensure_cache_size(7); /* guarantee capacity for first 8 values */

    /* Populate initial values once */
    if (cache[0] == 0 && n >= 0) { /* simple check to initialise only once */
        for (idx_t i = 0; i < 8 && i <= n; ++i)
            cache[i] = init_vals[i];
    }

    if (n < cache_size && n <= 7)
        return cache[n];
    if (n < 8)
        return cache[n]; /* should already be set */

    /* Compute values up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Ensure we have enough space */
        if (i >= cache_size)
            ensure_cache_size(i);

        /* Compute coefficients (unsigned, small) */
        unsigned long long u = (unsigned long long)i;
        unsigned int c1 = (unsigned int)(4 * u - 3);      /* 4n-3   */
        unsigned int c2 = (unsigned int)(2 * u - 3);      /* 2n-3   */
        unsigned int c3 = (unsigned int)(11 * (u - 3));   /* 11(n-3)*/
        unsigned int c4 = (unsigned int)(2 * u - 9);      /* 2n-9   */
        unsigned int c5 = (unsigned int)(4 * u - 21);     /* 4n-21  */
        unsigned int c6 = (unsigned int)(u - 6);          /* n-6    */

        /* Positive terms: (4n-3)a(n-1) + (4n-21)a(n-7) */
        seq_t term_pos1 = mul_small(cache[i-1], c1);
        seq_t term_pos2 = mul_small(cache[i-7], c5);
        seq_t pos_total = term_pos1 + term_pos2;

        /* Negative terms: sum of the other four */
        seq_t term_neg1 = mul_small(cache[i-2], c2);
        seq_t term_neg2 = mul_small(cache[i-4], c3);
        seq_t term_neg3 = mul_small(cache[i-6], c4);
        seq_t term_neg4 = mul_small(cache[i-8], c6);
        seq_t neg_total = term_neg1 + term_neg2 + term_neg3 + term_neg4;

        /* Numerator = pos_total - neg_total (safe: recurrence guarantees non-negative) */
        seq_t numerator = pos_total - neg_total;

        /* a(n) = numerator / n */
        cache[i] = numerator / (seq_t)i;
    }
    return cache[n];
}
