/*
 * A163226  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A163226:
 *  a(n) = 41*a(n-1)+41*a(n-2)+41*a(n-3)-861*a(n-4)   for n >= 5
 *  a(0)=1, a(1)=43, a(2)=1806, a(3)=75852, a(4)=3184881
 *
 *  This function computes the sequence for any non‑negative index n.
 *  It uses a static cache so calls may be in any order, and it
 *  runs in O(n) time overall.  All arithmetic uses the seq_t type,
 *  which is chosen per build.  No floating point or external
 *  libraries are required. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values          */
static size_t cache_size = 0; /* number of values stored         */
static size_t cache_cap  = 0; /* allocated capacity              */

/* Ensure that entries 0..n are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache to accommodate up to n. */
    if (cache_cap == 0) {
        /* Start with space for at least 5 slots (0..4). */
        cache_cap = (n + 1 > 5) ? (size_t)(n + 1) : 5;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        /* Initialise the first four terms. */
        cache[0] = 1;
        cache[1] = 43;
        cache[2] = 1806;
        cache[3] = 75852;
        cache_size = 4;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = ((size_t)(n + 1)) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t term;

        if (i == 4) {
            /* The numerator coefficient for t^4 is 1, so add 1 at n==4. */
            term = 41 * cache[i-1] + 41 * cache[i-2] + 41 * cache[i-3]
                   - 861 * cache[i-4] + 1;
        } else {
            term = 41 * cache[i-1] + 41 * cache[i-2] + 41 * cache[i-3]
                   - 861 * cache[i-4];
        }
        cache[i] = term;
    }
    cache_size = (size_t)(n + 1);
}

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 43;
    if (n == 2) return 1806;
    if (n == 3) return 75852;

    /* For all n >= 4 compute via the recurrence. */
    ensure_cache(n);
    return cache[n];
}
