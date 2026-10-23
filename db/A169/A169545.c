/*
 * A169545  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)+...+2*a(n-34)-3*a(n-35), with a(0)=1
 *            and a(n)+(-2)*a(n-1)+...+(-2)*a(n-34)+3*a(n-35)=0 for n>35
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache          = NULL;   /* cached values of a(n) */
static size_t cache_cap      = 0;      /* current capacity of cache, in elements */
static idx_t  cache_done     = 0;      /* highest index computed so far */

/* Ensure that cache can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((idx_t)(cache_cap) <= n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;

        seq_t *new_arr = seq_alloc_array(new_cap);
        if (cache)
        {
            memcpy(new_arr, cache, cache_cap * sizeof(seq_t));
            free_seq_array(cache);
        }
        cache      = new_arr;
        cache_cap  = new_cap;
    }
}

/* Return a(n) for the Coxeter group with relations (S_i)^2=(S_iS_j)^35=I */
seq_t a(idx_t n)
{
    /* first-time initialization */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = seq_alloc_array(cache_cap);
        memset(cache, 0, cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)1;
        cache_done = 0;
    }

    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t m = cache_done + 1; m <= n; ++m)
    {
        seq_t next = (seq_t)0;

        /* contributions from a(m-1) ... a(m-34) with coefficient 2 */
        for (idx_t k = 1; k <= 34; ++k)
        {
            if (k > m) break;
            next += (seq_t)2 * cache[m - k];
        }

        /* contribution from a(m-35) with coefficient -3 (only for m>=35) */
        if (m >= 35)
            next -= (seq_t)3 * cache[m - 35];

        /* add the right‑hand side N_m */
        if (m <= 34)
            next += (seq_t)2;
        else if (m == 35)
            next += (seq_t)1;
        /* for m>35, no addition */

        cache[m] = next;
        cache_done = m;
    }

    return cache[n];
}
