/*
 * A230450  "Comma sum" sequence: a(n+1)-a(n) = rightmost digit of a(n) +
 *          leftmost digit of a(n+1), use smallest possible solution; starting
 *          with 396.
 * https://oeis.org/A230450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 396;
   For n ≥ 1,
       let r = a(n) % 10;
       choose the smallest f in {1,…,9} such that the leftmost digit of
       a(n)+r+f equals f;
       then a(n+1) = a(n) + r + f. */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;        /* array of computed terms */
static idx_t cache_len = 0;        /* number of terms stored (≥1 after first use) */
static idx_t cache_cap = 0;        /* allocated capacity of cache */

/* Ensure cache has space for at least up_to terms */
static void ensure_cache_capacity(idx_t up_to)
{
    if (cache_cap >= up_to)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < up_to)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    /* for the purposes of this problem we assume realloc succeeds */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; n==0 is undefined, return 0 for safety */
        return (seq_t)0;

    /* Initialize cache with first term if needed */
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)396;      /* a(1) */
        cache_len = 1;
    }

    /* If requested term already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to n */
    ensure_cache_capacity(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];            /* a(i-1) */
        seq_t r    = prev % ((seq_t)10);      /* rightmost digit of a(i-1) */
        seq_t next = 0;                       /* will hold a(i) */

        /* Find smallest f (1..9) such that first digit of candidate = f */
        for (unsigned f = 1; f <= 9; ++f) {
            seq_t cand = prev + r + (seq_t)f;
            seq_t x = cand;
            while (x >= ((seq_t)10))
                x /= ((seq_t)10);            /* integer division by 10 */
            if (x == f) {
                next = cand;
                break;
            }
        }
        cache[i - 1] = next;                  /* store a(i) */
    }
    cache_len = n;
    return cache[n - 1];
}
