/*
 * A041247  Denominators of continued fraction convergents to sqrt(135).
 * https://oeis.org/A041247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 488*a(n-8) - a(n-16) (n ≥ 16) with a(0) = 1, a(1) = 1,
 *            a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 8, a(6) = 13, a(7) = 21,
 *            a(8) = 475, a(9) = 496, a(10) = 971, a(11) = 1467, a(12) = 2438,
 *            a(13) = 3905, a(14) = 6343, a(15) = 10248
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Local copy of the first 16 terms, as 64‑bit constants.  These will
   be cast to seq_t when the cache is initialised. */
static const uint64_t init_vals_ull[16] = {
    1ULL, 1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 13ULL, 21ULL,
    475ULL, 496ULL, 971ULL, 1467ULL, 2438ULL, 3905ULL,
    6343ULL, 10248ULL
};

/* Cache of computed terms.  All accessed through indices 0 … cache_size‑1. */
static seq_t *cache      = NULL;   /* dynamically allocated array of seq_t     */
static idx_t cache_size  = 0;      /* number of elements currently stored      */
static size_t cache_cap  = 0;      /* allocated capacity in elements           */

/* Ensure the cache can hold up to ‘needed’ inclusive.  Resizes as
   necessary by powers of two. */
static void ensure_cache_capacity(idx_t needed)
{
    if ((size_t)needed + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)needed + 1)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0)
        exit(EXIT_FAILURE);     /* out of memory */
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialise the cache with the first 16 known terms.  This is done
   lazily on the first call to a(n). */
static void init_cache(void)
{
    if (cache_size != 0)          /* already initialised */
        return;

    ensure_cache_capacity(15);    /* we need indices 0 … 15 */

    for (idx_t i = 0; i < 16; ++i)
        cache[i] = (seq_t)init_vals_ull[i];   /* cast to seq_t */

    cache_size = 16;
}

/* Compute term n ≥ 16 using the recurrence.  All operations are
   performed on type seq_t, which may be an unsigned integer type, or
   an arbitrary-precision class implementing + – * operators. */
static inline seq_t recurrence_term(seq_t prev8, seq_t prev16)
{
    return (seq_t)488 * prev8 - prev16;
}

/* External function: return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_size)
        return cache[n];

    /* Grow the cache so that index n is reachable. */
    ensure_cache_capacity(n);

    /* Compute missing terms in order up to n. */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* k ≥ 16 here because cache_size is 16 after init. */
        cache[k] = recurrence_term(cache[k - 8], cache[k - 16]);
    }
    cache_size = n + 1;    /* update number of stored terms */

    return cache[n];
}
