/*
 * A186240  G.f. A(x) defined by A(x) = 1 +x*A(x) +x^2*A(x)^2 +3*x^3*A(x)^3.
 * https://oeis.org/A186240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A186240 –  a(0)=1, a(1)=1, a(2)=2, a(3)=7
    6*(2*n+3)*(n+1)*a(n)+(277*n^2-191*n-162)*a(n-1)
      +6*(-155*n^2+305*n-126)*a(n-2)
      -4*(181*n-195)*(n-2)*a(n-3)
      -5304*(n-2)*(n-3)*a(n-4)=0   (n ≥ 4)   */
#include <stdlib.h>
#include <stdint.h>

/* cache for memoization – zero‑initialised */
static seq_t *cache = NULL;     /* cached a(0)…a(k) */
static size_t cache_size = 0;   /* allocated size of cache   */
static idx_t cached_n   = 0;    /* highest index already computed */

/* enlarge cache to hold up to index n (inclusive) */
static void
ensure_cache(idx_t n)
{
    /* one‑time initialisation: set base cases a(0)…a(3) */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cached_n = 3;
    }

    /* grow array if needed */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_sz = cache_size;
        while ((size_t)(n + 1) > new_sz) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_sz;
    }

    /* compute missing terms from cached_n+1 .. n */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        /* integer coefficients – pre‐compute with 64‑bit signed */
        int64_t k32 = (int64_t)k;
        int64_t coeff1 = 277 * k32 * k32 - 191 * k32 - 162;
        int64_t coeff2 = 6 * ( -155 * k32 * k32 + 305 * k32 - 126 );
        int64_t coeff3 = 4 * ( (181 * k32 - 195) * (k32 - 2) );   /* always positive */
        int64_t coeff4 = 5304 * (k32 - 2) * (k32 - 3);             /* always positive */

        /* denominator: 6*(2*k+3)*(k+1) */
        int64_t denom = 6 * (2 * k32 + 3) * (k32 + 1);

        /* ---------- 64‑bit seq_t : use __int128 for intermediate ---------- */
        if (sizeof(seq_t) <= 8) {
            __int128 term1 = (__int128)coeff1 * (__int128)cache[k-1];
            __int128 term2 = (__int128)coeff2 * (__int128)cache[k-2];
            __int128 term3 = (__int128)coeff3 * (__int128)cache[k-3];
            __int128 term4 = (__int128)coeff4 * (__int128)cache[k-4];

            /* numerator = -(term1 + term2) + term3 + term4  */
            __int128 num = -(term1 + term2) + term3 + term4;

            cache[k] = (seq_t)(num / (__int128)denom);
        }
        /* ---------- arbitrary‑precision seq_t : use seq_t arithmetic ---------- */
        else {
            /* start with the two positive terms (coeff3, coeff4) */
            seq_t num = (seq_t)coeff3 * cache[k-3] + (seq_t)coeff4 * cache[k-4];

            /* handle coeff1 (may be negative) */
            seq_t prod1 = (seq_t)(coeff1 >= 0 ? coeff1 : -coeff1) * cache[k-1];
            if (coeff1 >= 0)
                num -= prod1;      /* subtract positive coeff1 term */
            else
                num += prod1;      /* add because coeff1 is negative */

            /* handle coeff2 (may be negative) */
            seq_t prod2 = (seq_t)(coeff2 >= 0 ? coeff2 : -coeff2) * cache[k-2];
            if (coeff2 >= 0)
                num -= prod2;
            else
                num += prod2;

            /* final division */
            seq_t d = (seq_t)denom;
            cache[k] = num / d;
        }
    }

    cached_n = n;
}

/* public entry point – returns a(n) for all n ≥ 0 */
seq_t a(idx_t n)
{
    if (n <= 3) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)7;
        }
    }

    ensure_cache(n);
    return cache[n];
}
