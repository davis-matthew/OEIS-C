/*
 * A030982  Number of noncrossing nonplanted bushes with n nodes, i.e., rooted
 *          noncrossing trees with n nodes and no nodes of degree 1.
 * https://oeis.org/A030982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Sum_{k=2..n}
 *            ((-1)^(n-k)*2^(n-k)*k*C(n,k)*C(3*k-2,k-2)/(2*k+1))/n Recurrence
 *            (for n >= 5): 2*n*(2*n+1)*a(n) = (n-1)*(11*n-12)*a(n-1) +
 *            6*(9*n^2-21*n+8)*a(n-2) - 4*(n-3)*(11*n-56)*a(n-3) -
 *            152*(n-4)*(n-3)*a(n-4) Initial values: a(1)=0, a(2)=1, a(3)=1,
 *            a(4)=7
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Cached values and flags */
static seq_t *cache = NULL;
static bool   *computed = NULL;
static size_t cache_size = 0;
static idx_t  last_computed = 0;

static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();
        computed = (bool *)realloc(computed, new_size * sizeof(bool));
        if (!computed) abort();
        for (size_t i = cache_size; i < new_size; ++i)
            computed[i] = false;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, just return 0 */

    ensure_cache(n);
    if (n <= last_computed && computed[n]) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)0;
        } else if (i == 2) {
            val = (seq_t)1;
        } else if (i == 3) {
            val = (seq_t)1;
        } else if (i == 4) {
            val = (seq_t)7;
        } else { /* recurrence for i >= 5 */
#ifndef __cplusplus
            /* 64‑bit build: use 128‑bit intermediate to avoid overflow */
            typedef __int128 int128_t;
            int128_t coeff1 = (int128_t)(i - 1) * ((int128_t)11 * i - 12);
            int128_t coeff2 = (int128_t)6 * ((int128_t)9 * i * i - (int128_t)21 * i + 8);
            int128_t coeff3 = (int128_t)4 * ((int128_t)(i - 3) * ((int128_t)11 * i - 56));
            int128_t coeff4 = (int128_t)152 * ((int128_t)(i - 4) * (int128_t)(i - 3));

            int128_t pos = coeff1 * (int128_t)cache[i - 1] + coeff2 * (int128_t)cache[i - 2];
            int128_t neg = coeff3 * (int128_t)cache[i - 3] + coeff4 * (int128_t)cache[i - 4];
            int128_t rhs = pos - neg;
            int128_t denom = (int128_t)(2 * i) * (int128_t)(2 * i + 1);
            val = (seq_t)(rhs / denom);
#else
            /* C++ build: seq_t is a big‑integer class, so we can use its operators */
            seq_t coeff1 = (seq_t)(i - 1) * ((seq_t)11 * i - 12);
            seq_t coeff2 = (seq_t)6 * ((seq_t)9 * i * i - (seq_t)21 * i + 8);
            seq_t coeff3 = (seq_t)4 * ((seq_t)(i - 3) * ((seq_t)11 * i - 56));
            seq_t coeff4 = (seq_t)152 * ((seq_t)(i - 4) * (seq_t)(i - 3));

            seq_t pos = coeff1 * cache[i - 1] + coeff2 * cache[i - 2];
            seq_t neg = coeff3 * cache[i - 3] + coeff4 * cache[i - 4];
            seq_t rhs = pos - neg;
            seq_t denom = (seq_t)(2 * i) * (seq_t)(2 * i + 1);
            val = rhs / denom;
#endif
        }
        cache[i] = val;
        computed[i] = true;
    }
    last_computed = n;
    return cache[n];
}
