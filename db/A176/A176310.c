/*
 * A176310  G.f.: exp( Sum_{n>=1} sigma(n*2^n)*x^n/n ).
 * https://oeis.org/A176310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, for n≥1
   a(n) = (1/n) * Σ_{k=0}^{n-1} a(k) * σ((n−k)·2^(n−k))   where σ(m)=sum of divisors of m */

#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/pow2.h"

static seq_t *a_cache      = NULL; /* a[n] values, a_cache[0]=1 */
static seq_t *c_cache      = NULL; /* σ(n·2^n) values, c_cache[0] unused */
static idx_t  cache_max    = 0;     /* highest n stored */
static size_t cache_cap    = 0;     /* allocated size */

/* Return the nth term of OEIS A176310. */
seq_t a(idx_t n)
{
    /* Fast return for n==0 */
    if (n == 0) return (seq_t)1;

    /* Allocate or grow cache if necessary */
    if (a_cache == NULL) {
        cache_cap = n + 1;
        a_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        c_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (a_cache == NULL || c_cache == NULL) return (seq_t)0;
        a_cache[0] = (seq_t)1;        /* a(0)=1 */
        c_cache[0] = (seq_t)0;        /* unused */
        cache_max = 0;
    } else if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        c_cache = (seq_t *)realloc(c_cache, new_cap * sizeof(seq_t));
        if (a_cache == NULL || c_cache == NULL) return (seq_t)0;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already done */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Compute σ(i·2^i) */
        idx_t m = i;
        idx_t v = 0;
        while ((m & 1ULL) == 0) {   /* v = v₂(i) */
            m >>= 1;
            ++v;
        }
        idx_t odd = m;              /* odd part of i */
        seq_t sigma_odd = sigma_of(odd);

        /* σ(2^(i+v)) = 2^(i+v+1) – 1 */
        seq_t pow2_val = pow2((idx_t)(i + v + 1)); /* 2^(i+v+1) */
        seq_t sigma_power = pow2_val - (seq_t)1;

        /* σ(i·2^i) = σ(odd) * (2^(i+v+1) – 1) */
        seq_t sigma_val = sigma_odd * sigma_power;
        c_cache[i] = sigma_val;

        /* Recurrence: a(i) = (1/i) * Σ_{k=0}^{i-1} a(k) * c[i−k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            sum += a_cache[k] * c_cache[i - k];
        }
        a_cache[i] = sum / (seq_t)i;
    }

    cache_max = n;
    return a_cache[n];
}
