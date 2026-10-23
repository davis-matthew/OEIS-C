/*
 * A143754  Aliquot sequence starting at 75.
 * https://oeis.org/A143754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 75, * a(n+1) = σ(a(n)) – a(n) (sum of proper divisors)
 *            * This is the aliquot sequence starting at 75
 * Range:     u8 n <= 0, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t x) */

/* Cache for already computed terms */
static seq_t *cache = NULL;   /* array storing the sequence values */
static size_t cache_size = 0; /* number of terms currently stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* Compute the sum of proper divisors of x (x > 1).  For x ≤ 1 the sum is 0. */
static seq_t proper_divisor_sum(seq_t x)
{
    if (x <= 1) return 0;              /* 0 or 1 has no proper divisors */

    seq_t sum = 1;                     /* 1 is always a proper divisor */
    seq_t root = floor_sqrt(x);        /* integer square root */

    /* Loop from 2 up to root inclusive.  Using seq_t for the loop counter
     * works for all supported seq_t types (built‑in integer or arbitrary
     * precision).  The loop terminates even for small seq_t values. */
    for (seq_t d = 2; d <= root; ++d) {
        if ((x % d) == 0) {
            seq_t q = x / d;
            sum += d;                  /* add the divisor d */
            if (q != d) sum += q;      /* add the paired divisor if distinct */
        }
    }
    return sum;
}

/* Ensure that cache contains all terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Lazy allocation of the cache.  The initial capacity is a small power
     * of two to keep resizing cheap. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);   /* allocation failed */
        cache[0] = 75;                /* a(0) = 75 */
        cache_size = 1;
    }

    /* Grow the cache if necessary.  The new capacity is the smallest power
     * of two that can hold indices up to n. */
    size_t needed = (size_t)n + 1;    /* indices are 0‑based */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms.  cache_size holds the first index that
     * is not yet computed, so we start from that index. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = proper_divisor_sum(prev);
    }
    cache_size = (size_t)n + 1;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
