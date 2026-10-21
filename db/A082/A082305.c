/*
 * A082305  G.f.: (1 - 6*x - sqrt(36*x^2 - 16*x + 1))/(2*x).
 * https://oeis.org/A082305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*Sum_{k=0..n} 7^k*C(n,k)*C(n,k-1) * Recurrence:
 *            (n+1)*a(n) + 8*(1-2*n)*a(n-1) + 36*(n-2)*a(n-2) = 0 * initial
 *            values: a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Ensure that cache[0..n] exist and are computed. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n) return;               /* already present */

    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* seed values */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        if (new_size > 1) cache[1] = (seq_t)7;
    } else if (cache_size == 1 && new_size > 1) {
        cache[1] = (seq_t)7;
    }

    /* compute new terms */
    for (idx_t i = (idx_t)cache_size; i < (idx_t)new_size; ++i)
    {
        if (i == 0) { cache[i] = (seq_t)1; continue; }
        if (i == 1) { cache[i] = (seq_t)7; continue; }

        /* coefficients */
        unsigned long long coeff1 = 8ULL * ((unsigned long long)(2ULL * i - 1ULL));
        unsigned long long coeff2 = 36ULL * ((unsigned long long)(i >= 2 ? i - 2ULL : 0ULL));

#if defined(__cplusplus)
        /* C++ path – use seq_t arithmetic (big‑int or built‑in) */
        seq_t term1 = cache[i-1] * coeff1;
        seq_t term2 = (i >= 2) ? (cache[i-2] * coeff2) : 0;
        seq_t numer = term1 - term2;
        cache[i] = numer / ((seq_t)(i + 1));
#else
        /* C path – protect intermediate overflow with 128‑bit arithmetic */
        __uint128_t prod1 = (__uint128_t)cache[i-1] * coeff1;
        __uint128_t prod2 = (__uint128_t)cache[i-2] * coeff2;
        __uint128_t numer = prod1 - prod2;
        cache[i] = (seq_t)(numer / (i + 1));
#endif
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
