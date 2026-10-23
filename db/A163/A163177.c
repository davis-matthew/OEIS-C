/*
 * A163177  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=26, a(2)=650, a(3)=16250, a(4)=405925 a(n) =
 *            24*a(n-1) + 24*a(n-2) + 24*a(n-3) - 300*a(n-4) (n ≥ 5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the linear recurrence obtained from the
   rational generating function.  It caches previously computed values
   so that repeated calls to a(n) are O(1).  The code is valid both in
   C11 and C++17.  Casts to (seq_t) are used for the small integer
   constants to handle all supported seq_t types.  */
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* array of computed values           */
static idx_t  cached_len = 0;     /* number of valid entries in cache   */
static size_t cached_cap = 0;     /* allocated capacity of the cache    */

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cached_len > n) return;

    /* Resize the cache if necessary.  Grow to the next power of two. */
    idx_t needed = n + 1;
    size_t new_cap = cached_cap;
    if (new_cap == 0) new_cap = 8;
    while ((idx_t)new_cap < needed) new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    assert(cache != NULL);
    cached_cap = new_cap;

    /* Compute values from cached_len up to n.  */
    for (idx_t i = cached_len; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)26;
        else if (i == 2)
            cache[i] = (seq_t)650;
        else if (i == 3)
            cache[i] = (seq_t)16250;
        else if (i == 4)
            cache[i] = (seq_t)405925;
        else
        {
            seq_t t1 = (seq_t)24 * cache[i-1];
            seq_t t2 = (seq_t)24 * cache[i-2];
            seq_t t3 = (seq_t)24 * cache[i-3];
            seq_t t4 = (seq_t)300 * cache[i-4];
            cache[i] = t1 + t2 + t3 - t4;   /* recursive step for n ≥ 5 */
        }
    }

    cached_len = n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
