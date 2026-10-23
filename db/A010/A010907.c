/*
 * A010907  Pisot sequence E(4,19), a(n) = floor( a(n-1)^2/a(n-2)+1/2 ).
 * https://oeis.org/A010907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) + 2*a(n-3) + a(n-4) a(0) = 4, a(1) =
 *            19, a(2) = 90, a(3) = 426
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of previously computed terms */
static seq_t *cache      = NULL; /* pointer to array of seq_t values */
static idx_t  max_computed = (idx_t)-1; /* index of last computed term */
static idx_t  capacity     = 0;        /* allocated array capacity */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (capacity <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }
}

/* Initialise cache with the first four sequence values.
   This function is idempotent. */
static void init_base_terms(void)
{
    if (cache != NULL) {
        return; /* already initialised */
    }
    capacity = 4;
    cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)19;
    cache[2] = (seq_t)90;
    cache[3] = (seq_t)426;
    max_computed = 3;
}

/* Compute a(n) for any non‑negative n, memoising the intermediate
   values. The recurrence is first‑order in terms of the cached
   array, giving O(n) total work. */
seq_t a(idx_t n)
{
    init_base_terms();

    if (n <= max_computed) {
        return cache[n];
    }

    ensure_capacity(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)4 * cache[i - 1] +
            (seq_t)3 * cache[i - 2] +
            (seq_t)2 * cache[i - 3] +
            cache[i - 4];
    }

    max_computed = n;
    return cache[n];
}
