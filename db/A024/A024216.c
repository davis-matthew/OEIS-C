/*
 * A024216  a(n) = n-th elementary symmetric function of the first n+1
 *          positive integers congruent to 1 mod 3.
 * https://oeis.org/A024216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5; for n ≥ 2, a(n) = (6*(n-1)+5)*a(n-1) -
 *            (3*(n-1)+1)^2 * a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* Ensure the cache can hold the term at index n */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = ((size_t)n + 1) * 2;
        if (new_cap < 4) new_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute the n‑th term of A024216 */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    /* Initialise first terms if not already done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (n >= 1 && cache_size == 1) {
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        idx_t k = i - 1;           /* k = n-1 for the recurrence */
        seq_t m1 = (seq_t)(6 * k + 5);
        seq_t m2 = (seq_t)(3 * k + 1);
        seq_t term1 = cache[k] * m1;
        seq_t term2 = cache[k - 1] * m2 * m2;
        cache[i] = term1 - term2;
        cache_size = i + 1;
    }
    return cache[n];
}
