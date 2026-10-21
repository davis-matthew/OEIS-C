/*
 * A050032  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique value such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 3.
 * https://oeis.org/A050032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 194, u64 n <= 769,
 *            u128 n <= 3624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050032
 * Formula: a(1)=1, a(2)=1, a(3)=3; for n≥4,
 * a(n) = a(n-1)+a(m) with m = 2*n-3-2^(p+1)
 * where p = floor(log₂(n‑2))  (i.e. largest p with 2^p < n‑1 ≤ 2^(p+1)).
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[1]..cache[n] hold sequence values */
static idx_t cache_cap = 0;          /* current allocated size of cache (number of elements) */
static idx_t max_computed = 0;       /* highest n for which cache[i] is defined */

/* Ensure cache has room for indices up to n (inclusive).  New elements are left
 * uninitialised; they will be filled when we compute the sequence.  */
static void ensure_cache(idx_t n)
{
    if (cache && n < cache_cap) return;
    idx_t new_cap = n + 1; /* store indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the sequence values up to and including index n, storing them in cache. */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;      /* already computed */
    ensure_cache(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)1;
        } else if (i == 3) {
            cache[3] = (seq_t)3;
        } else {
            /* p = floor(log2(i-2)) */
            unsigned int p = 63 - __builtin_clzll(i-2);
            /* pow = 2^(p+1) as an index value */
            idx_t pow_idx = 1ULL << (p + 1);
            /* m = 2*i - 3 - pow_idx   (guaranteed m ≥ 1 and m ≤ i-2) */
            idx_t m = (idx_t)(2 * i - 3) - pow_idx;
            cache[i] = cache[i - 1] + cache[m];
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) abort(); /* undefined for n<1 */
    compute_up_to(n);
    return cache[n];
}
