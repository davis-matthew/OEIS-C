/*
 * A202850  Number of secondary structures of size n with no stacks of length
 *          >=2.
 * https://oeis.org/A202850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 7,
 *            a(6) = 14 * For n ≥ 7: * a(n) = ( (2n+1)a(n-1) - (n-1)a(n-2) +
 *            3(2n-5)a(n-3) * - 2(n-4)a(n-4) + 2(2n-11)a(n-5) - (n-7)a(n-6) )
 *            / (n+2) * This is the recurrence extracted from the OEIS entry.
 *            * * The implementation memoises results in a...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;     /* capacity of cache */
static idx_t cached_max  = (idx_t)-1; /* largest n that has been computed */

/* Ensure that cache contains entries up to 'n' (inclusive) and
 * initialise the base values a(0)..a(6). */
static void ensure_cache(idx_t n)
{
    /* Allocate or enlarge the cache if necessary */
    if (cache == NULL) {
        /* Allocate at least up to index n, but always keep 7 slots for the base cases */
        size_t cap = (size_t)(n >= 6 ? n + 1 : 7);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();

        /* Base values */
        const seq_t base[7] = {
            (seq_t)1, (seq_t)1, (seq_t)1,
            (seq_t)2, (seq_t)4, (seq_t)7, (seq_t)14
        };
        for (idx_t i = 0; i <= 6; ++i) cache[i] = base[i];

        cache_cap  = cap;
        cached_max = 6;   /* base values precomputed */
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* no need to touch cached_max here */
    }
}

/* Public function implementing the recurrence */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Returning a pre‑computed value if it exists */
    if (n <= cached_max) return cache[n];

    /* Compute values for indices greater than cached_max up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 6) {
            /* Base case already stored during allocation */
            continue;
        }

        /* Recurrence:
         * a(i) = ((2i+1)a(i-1) - (i-1)a(i-2) + 3(2i-5)a(i-3)
         *        - 2(i-4)a(i-4) + 2(2i-11)a(i-5) - (i-7)a(i-6)) / (i+2) */
        seq_t t1 = (seq_t)(2 * i + 1) * cache[i - 1];
        seq_t t2 = (seq_t)(i - 1) * cache[i - 2];
        seq_t t3 = (seq_t)3 * (2 * i - 5) * cache[i - 3];
        seq_t t4 = (seq_t)2 * (i - 4) * cache[i - 4];
        seq_t t5 = (seq_t)2 * (2 * i - 11) * cache[i - 5];
        seq_t t6 = (seq_t)(i - 7) * cache[i - 6];

        seq_t num = t1 + t3 + t5;  /* add positive terms */
        num -= t2;                 /* subtract negative terms */
        num -= t4;
        num -= t6;

        cache[i] = num / (seq_t)(i + 2);
    }

    cached_max = n;
    return cache[n];
}
