/*
 * A229820  Even bisection gives sequence a itself, n->a(2*(5*n+k)-1) gives
 *          k-th differences of a for k=1..5 with a(n)=n for n<2.
 * https://oeis.org/A229820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(2*n) = a(n), a(2*(5*n+k)-1) = Sum_{j=0..k} (-1)^j * C(k,j) *
 *            a(n + k - j) for k=1..5 with a(n) = n for n < 2. An on-demand
 *            memoised recursion is used
 * Range:     i8 n <= 68, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values */
static unsigned char *seen = NULL;  /* 0 = not computed, 1 = computed */
static size_t cache_cap = 0;   /* current capacity */

/* binomial coefficients C[k][j] for 0 <= k,j <= 5 */
static const int C[6][6] = {
    {1,0,0,0,0,0},
    {1,1,0,0,0,0},
    {1,2,1,0,0,0},
    {1,3,3,1,0,0},
    {1,4,6,4,1,0},
    {1,5,10,10,5,1}
};

/* Ensure that cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *new_seen = (unsigned char *)realloc(seen, new_cap);
    if (!new_cache || !new_seen) /* allocation failed – fall back to no memoisation */
        return;
    cache = new_cache;
    seen = new_seen;
    for (size_t i = cache_cap; i < new_cap; ++i)
        seen[i] = 0;
    cache_cap = new_cap;
}

/* Recursive memoised computation of a(n). */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)n;

    ensure_cache(n);
    size_t idx = (size_t)n;
    if (seen[idx]) return cache[idx];

    seq_t res;
    if ((n & 1ULL) == 0) {
        res = a(n >> 1);
    } else {
        /* Find k (1..5) such that (n+1)/2 - k is divisible by 5 (and non‑negative). */
        idx_t s = (n + 1) / 2;
        int k_found = -1;
        idx_t m = 0;
        for (int k = 1; k <= 5; ++k) {
            if (s < (idx_t)k) continue;
            idx_t diff = s - (idx_t)k;
            if (diff % 5ULL == 0) {
                m = diff / 5ULL;
                k_found = k;
                break;
            }
        }
        /* k_found must exist for all odd n >= 3; if not, default to recursive path. */
        if (k_found == -1) {
            /* Fallback – treat as if it were even (should never happen). */
            res = a(n >> 1);
        } else {
            int k = k_found;
            seq_t sum = 0;
            for (int j = 0; j <= k; ++j) {
                seq_t term = a(m + (idx_t)(k - j));
                seq_t coef = (seq_t)C[k][j];
                if ((j & 1) == 0) sum += term * coef;
                else sum -= term * coef;
            }
            res = sum;
        }
    }

    seen[idx] = 1;
    cache[idx] = res;
    return res;
}
