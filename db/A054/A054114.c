/*
 * A054114  T(2n+1,n), array T as in A054110.
 * https://oeis.org/A054114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 * a(n) = ∑_{k=1}^{n+1} C(2k,k) if n > 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static memoisation state */
static seq_t *cache = NULL;      /* array of a(n) values */
static size_t cache_len = 0;     /* number of cached entries */
static size_t cache_cap = 0;     /* current allocation size */

/* State for incremental binomial and sum computation */
static idx_t  curr_k      = 0;      /* last computed binomial index k (starts 0) */
static seq_t  last_binom  = 1;      /* C(2*curr_k, curr_k), last_binom=1 for k=0 */
static seq_t  curr_sum    = 0;      /* sum_{i=1}^{curr_k} C(2i,i) */

/* Ensure the cache has at least 'need' slots (need is size, not index). */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < need)
        cache_cap <<= 1;

    seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!newp)
        abort();                      /* out of memory */
    cache = newp;
}

/* Make sure values up to index n are available in the cache. */
static void ensure_up_to(idx_t n)
{
    /* Initialise a(0) if not already done */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n == 0)
        return;

    /* We need binomial values up to k = n+1 */
    idx_t target_k = n + 1;

    /* Allocate cache storage up to n (inclusive) */
    ensure_cache_cap(n + 1);

    /* Incrementally compute missing binomial values */
    while (curr_k < target_k) {
        ++curr_k;                       /* now curr_k >= 1 */

        /* Compute C(2*curr_k, curr_k) from the previous value.
           Formula: C(2k,k) = C(2(k-1),k-1) * (4k - 2) / k  */
        seq_t k_seq = (seq_t)curr_k;
        seq_t mul  = last_binom * ((k_seq << 2) - (seq_t)2);
        seq_t new_binom = mul / k_seq;

        last_binom = new_binom;
        curr_sum  += new_binom;

        /* Store a(k-1) = sum_{i=1}^{k} C(2i,i) for k >= 2.
           a(1) is stored when curr_k == 2, a(2) when curr_k == 3, ... */
        if (curr_k >= 2) {
            size_t idx = curr_k - 1;     /* index for a(curr_k-1) */
            cache[idx] = curr_sum;
            if (idx + 1 > cache_len)
                cache_len = idx + 1;
        }
    }
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
