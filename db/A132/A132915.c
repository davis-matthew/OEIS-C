/*
 * A132915  a(0)=0; a(1)=1; a(n) = Sum_{k=1..[sqrt(n)]} a(n-k) for n>=2.
 * https://oeis.org/A132915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; a(n) = Σ_{k=1..⌊√n⌋} a(n-k) for n ≥ 2
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute ⌊√v⌋ for a 64‑bit unsigned integer v.  */
static idx_t sqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq = mid * mid;          /* mid ≤ 2³²−1, so mid*mid fits in 64 bits */
        if (sq <= v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

static seq_t *cache = NULL;       /* computed sequence values */
static idx_t  cache_size = 0;     /* allocated size of cache array  */
static idx_t  next_compute = 0;   /* first index that has not been computed yet */

static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_size = (idx_t)new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);

    /* initialize base cases once */
    if (next_compute <= 1) {
        cache[0] = (seq_t)0;
        if (cache_size > 1) cache[1] = (seq_t)1;
        next_compute = 2;
    }

    /* compute values up to n, if needed */
    for (idx_t i = next_compute; i <= n; ++i) {
        idx_t d = sqrt_u64(i);
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= d; ++k) {
            sum += cache[i - k];
        }
        cache[i] = sum;
    }
    next_compute = n + 1;

    return cache[n];
}
