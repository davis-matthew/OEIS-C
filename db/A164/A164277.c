/*
 * A164277  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*Σ_{k=1..5} a(n-k) - 903*a(n-6), n >= 7 * a(0) = 1 *
 *            a(1) = 44 * a(2) = 1892 * a(3) = 81356 * a(4) = 3498308 * a(5) =
 *            150427244 * The value a(6) follows from the same recurrence with
 *            an additional * constant term: a(6) = 1 + 42*Σ_{k=1..5} a(6-k) -
 *            903*a(0). This * yields the reported a(6...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Static cache for computed terms; dynamically resized as needed */
static seq_t *cache     = NULL;  /* array of seq_t */
static idx_t  cache_len = 0;     /* current allocated length (>= num stored terms) */
static idx_t  last_computed = 5; /* highest index already computed; init to 5 */

/* Ensure that cache has entries up to index n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_len = new_len;
    }
}

/* Initialize cache with the first seven terms */
static void init_cache(void)
{
    if (cache == NULL) {
        size_t init_len = 7;                 /* a0 .. a6 */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_len = init_len;
    }
    /* Known initial terms */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)44;
    cache[2] = (seq_t)1892;
    cache[3] = (seq_t)81356;
    cache[4] = (seq_t)3498308;
    cache[5] = (seq_t)150427244;
    /* Compute a(6) using the special formula with constant 1 */
    seq_t sum = cache[5] + cache[4] + cache[3] + cache[2] + cache[1];
    cache[6] = sum * (seq_t)42 - cache[0] * (seq_t)903 + (seq_t)1;
    last_computed = 6;
}

/* Compute terms up to n using the recurrence for n >= 7 */
static void compute_to(idx_t n)
{
    /* If n <= last_computed, nothing to do */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
        cache[i] = sum * (seq_t)42 - cache[i-6] * (seq_t)903;
    }
    last_computed = n;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    /* First call: initialise cache */
    if (cache == NULL) init_cache();

    /* Extend cache if necessary */
    if (n >= cache_len) ensure_cache_size(n);

    /* If the term already exists, return it */
    if (n <= last_computed) return cache[n];

    /* Compute remaining terms up to n */
    compute_to(n);
    return cache[n];
}
