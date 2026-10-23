/*
 * A015391  Gaussian binomial coefficient [ n,10 ] for q=-5.
 * https://oeis.org/A015391
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..10} ((-5)^(n-i+1)-1)/((-5)^i-1)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 11, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */

/* Cache of previously computed values (index 10 is the first defined term). */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;

/* Highest n for which cache[n] is known (initially 9, meaning nothing ≥10 computed). */
static idx_t max_computed = 9;

/* 5^max_computed  and  5^(max_computed-10) for the most recent n processed. */
static seq_t pow5_at_max      = 0;  /* 5^max_computed, valid when max_computed ≥ 10 */
static seq_t pow5_sub_at_max  = 0;  /* 5^(max_computed-10) */

/* Ensure the cache array is large enough to hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return;            /* allocation failure – caller will receive 0 */

    if (cache) {
        for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
    } else {
        for (size_t i = 0; i < new_size; ++i) new_cache[i] = 0;
    }

    cache      = new_cache;
    cache_size = (idx_t)new_size;
}

/* Gaussian binomial coefficient [n,10] for q = -5. */
seq_t a(idx_t n)
{
    if (n < 10) return 0;          /* undefined for indices below the offset */

    ensure_cache(n);

    /* Initialise the base case on the first call that needs it. */
    if (cache[10] == 0) {
        cache[10] = (seq_t)1;

        /* Compute 5^10 for the starting power. */
        seq_t p = (seq_t)1;
        for (int i = 0; i < 10; ++i) p *= (seq_t)5;
        pow5_at_max     = p;              /* 5^10  */
        pow5_sub_at_max = (seq_t)1;       /* 5^0   */

        max_computed = 10;
    }

    /* If requested value is already computed, return it. */
    if (n <= max_computed) return cache[n];

    /* Compute values up to n sequentially. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* Update the powers of 5. */
        pow5_at_max    *= (seq_t)5;          /* 5^k          */
        pow5_sub_at_max*= (seq_t)5;          /* 5^(k-10)     */

        /* Absolute numerators and denominators (signs cancel). */
        seq_t abs_num = ((k % 2) == 0)
                        ? (pow5_at_max - (seq_t)1)
                        : (pow5_at_max + (seq_t)1);

        seq_t abs_den = (((k - 10) % 2) == 0)
                        ? (pow5_sub_at_max - (seq_t)1)
                        : (pow5_sub_at_max + (seq_t)1);

        /* Recurrence: a(k) = a(k-1) * abs_num / abs_den */
        cache[k] = cache[k - 1] * abs_num / abs_den;
    }

    max_computed = n;
    return cache[n];
}
