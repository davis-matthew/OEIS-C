/*
 * A168440  a(n) = Product_{k=0..n} ((4*k+1)*(4*k+3))^(n-k).
 * https://oeis.org/A168440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} ((4*k+1)*(4*k+3))^(n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of a(n); a_cache[0] = a(0) = 1 */
static seq_t *a_cache  = NULL;
/* Number of terms currently stored (>=1 after initialization) */
static idx_t   cached_len = 0;
/* Current capacity of a_cache in terms of number of elements */
static idx_t   cached_cap = 0;
/* Cumulative product of base_k = (4*k+1)*(4*k+3) up to k=cached_len-1.
   Initially 1 before any base has been multiplied. */
static seq_t   prefix_prod = 1;

/* Ensure that a_cache can store values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cached_cap <= n) {
        idx_t new_cap = cached_cap ? cached_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        cached_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = 1;          /* a(0) = 1 */
        cached_len = 1;
        cached_cap = 1;
        prefix_prod = 1;
    }

    /* Return cached value if already computed */
    if (n < cached_len) return a_cache[n];

    /* Grow cache so we can compute through n */
    ensure_capacity(n);

    /* Compute missing terms */
    for (idx_t k = cached_len; k <= n; ++k) {
        /* base corresponding to index k-1 */
        seq_t base = ((seq_t)(4*(k-1) + 1)) * (seq_t)(4*(k-1) + 3);
        /* Update cumulative product of bases up to k-1 */
        prefix_prod *= base;
        /* Recursive relation: a(k) = a(k-1) * prefix_prod */
        a_cache[k] = a_cache[k-1] * prefix_prod;
    }

    cached_len = n + 1;
    return a_cache[n];
}
