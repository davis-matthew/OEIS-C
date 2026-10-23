/*
 * A154150  Numbers k such that 24 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154150: k such that 24 + T_k is a perfect square
   where T_k = k(k+1)/2.
   Sequence satisfies the 5‑term recurrence:
   a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - a(n-4) + a(n-5)
   with initial terms a(1)=1, a(2)=15, a(3)=24, a(4)=94, a(5)=145. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache[a(n-1)] = a(n) */
static idx_t   cache_size = 0;       /* number of terms stored */
static idx_t   cache_cap  = 0;       /* allocated capacity */

/* Ensure cache has room for at least `need` terms. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = need * 2;
    if (new_cap == 0)
        new_cap = 4;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* leave old cache unchanged on allocation failure */

    if (cache && cache_size)
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
    free(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialise the first five terms of the sequence. */
static void init_initial_terms(void)
{
    ensure_capacity(5);
    cache[0] = (seq_t)1;   /* a(1) */
    cache[1] = (seq_t)15;  /* a(2) */
    cache[2] = (seq_t)24;  /* a(3) */
    cache[3] = (seq_t)94;  /* a(4) */
    cache[4] = (seq_t)145; /* a(5) */
    cache_size = 5;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n=0 */

    /* Ensure first five terms are present. */
    if (cache_size < 5)
        init_initial_terms();

    /* If the requested term is already cached, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to accommodate up to n terms. */
    ensure_capacity(n);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* i corresponds to term a(i).  cache indices are
           offset by -1: cache[i-1] = a(i). */
        seq_t v = cache[i - 2];                         /* a(i-1)   */
        v += (seq_t)6 * cache[i - 3];                  /* +6*a(i-2)*/
        v -= (seq_t)6 * cache[i - 4];                  /* -6*a(i-3)*/
        v -= cache[i - 5];                            /* -a(i-4)  */
        v += cache[i - 6];                            /* +a(i-5)  */
        cache[i - 1] = v;
    }

    cache_size = n;
    return cache[n - 1];
}
