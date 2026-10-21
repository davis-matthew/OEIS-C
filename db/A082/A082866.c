/*
 * A082866  a(1)=1, a(2)=2, otherwise a(n) is the sum of the preceding terms
 *          a(j), 1<=j<n, where gcd(n,j)=1.
 * https://oeis.org/A082866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 78, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082866:
 * a(1) = 1, a(2) = 2, and for n > 2:
 *     a(n) = Σ_{1 ≤ j < n, gcd(n,j)=1} a(j) */

/* Use a cached array so calls can be made in any order. */
#include <stdint.h>
#include <stdlib.h>

/* Euclidean gcd for idx_t (uint64_t). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache storage: index 0 unused. */
static seq_t *cache = NULL;      /* dynamic array of values */
static idx_t cache_cap   = 0;    /* allocated capacity (≥ required + 1) */
static idx_t computed_to = 0;    /* highest index that has been computed */

/* Ensure the cache can hold indices up to n, and initialise base cases. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;  /* already large enough */

    /* Grow to a power of two at least n+1. */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out‑of‑memory – terminate. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;

    /* Initialise base values if not already done. */
    if (computed_to < 1)
        cache[1] = (seq_t)1;
    if (computed_to < 2)
        cache[2] = (seq_t)2;

    /* Ensure computed_to at least 2 after initialisation. */
    if (computed_to < 2)
        computed_to = 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined but harmless */

    ensure_cache(n);

    if (n <= computed_to)
        return cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t k = computed_to + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < k; ++j) {
            if (gcd_idx(k, j) == 1)
                sum += cache[j];
        }
        cache[k] = sum;
    }
    computed_to = n;

    return cache[n];
}
