/*
 * A202143  G.f. 1/[Sum_{n>=0} (2*n+1)*(-x)^(n*(n+1)/2)].
 * https://oeis.org/A202143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the power series A(x) = 1 / Σ_{k≥0}
 *            (2k+1)·(-x)^{k(k+1)/2}
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence with a linear-time
   recurrence derived from the condition A(x)·S(x)=1, where
   S(x)=Σ (2k+1)·(-x)^{k(k+1)/2}.  Only the coefficients of
   S(x) at triangular exponents are non‑zero.  The recurrence
   is
        a(0) = 1,
        a(n) = Σ_{k} a(n−T_k)·(2k+1)·s_k   (k≥1),
   where T_k = k(k+1)/2 and s_k = +1 if k≡0 or 3 (mod 4),
   otherwise s_k = –1.  This is implemented with two
   running sums to avoid intermediate negative values.  The
   sequence is memoised in a static cache so calls to a(n)
   in arbitrary order are answered in O(n) overall time. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* cache holds computed values a[0], a[1], …, a[cache_len-1] */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values currently cached */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_cache(size_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= n + 1) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();                     /* allocation failed */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n).  The function is safe for any n≥0 and works
   regardless of the order of calls. */
seq_t a(idx_t n)
{
    /* initialise cache with a(0)=1 if needed */
    if (n == 0) {
        ensure_cache(0);
        if (cache_len == 0) {
            cache[0] = 1;
            cache_len = 1;
        }
        return 1;
    }

    /* make sure space is available for a[n] */
    ensure_cache((size_t)n);

    /* compute from the first missing entry up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t t = k * (k + 1) / 2;          /* triangular number */
            if (t > i) break;
            seq_t term = cache[i - t];
            term *= (2 * k + 1);                /* multiply by (2k+1) */
            /* sign is +1 if k≡0 or 3 (mod 4), else –1 */
            if ((k & 3) == 0 || (k & 3) == 3)
                sum_pos += term;
            else
                sum_neg += term;
        }
        /* a[i] = sum_neg – sum_pos (non‑negative) */
        cache[i] = sum_neg - sum_pos;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
