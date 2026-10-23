/*
 * A022032  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(5,26).
 * https://oeis.org/A022032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=5, a(1)=26; for n≥1, a(n+1)=ceil(a(n)^2/a(n-1))-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses a simple memoised recurrence.
   It works for any unsigned integer type seq_t (8/16/32/64/128 bits)
   and also for arbitrary‑precision integer classes when compiled as C++.  */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Static cache for previously computed terms.                          */
/* --------------------------------------------------------------------- */
static seq_t *term_cache = NULL;   /* array of computed terms          */
static size_t cache_size  = 0;      /* number of terms currently stored */
static size_t cache_cap   = 0;      /* allocated capacity of term_cache */

/* --------------------------------------------------------------------- */
/*  Ensure the cache can hold terms up to index 'n'.                     */
/* --------------------------------------------------------------------- */
static void
ensure_cache(idx_t n)
{
    /* Grow capacity if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        term_cache = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i == 0) {
            term_cache[0] = (seq_t)5;
        } else if (i == 1) {
            term_cache[1] = (seq_t)26;
        } else {
            seq_t a = term_cache[i-1];
            seq_t b = term_cache[i-2];

#ifdef __cplusplus
            /* C++ build: rely on big‑integer operators. */
            seq_t prod = a * a;
            seq_t q = prod / b;
            if (prod % b != 0) ++q;          /* ceil */
            term_cache[i] = q - 1;
#else
/* C build: use a wider intermediate type to avoid overflow. */
# if defined(__SIZEOF_INT128__)
            /* 128‑bit arithmetic is available. */
            typedef unsigned __int128 uint128_t;
            uint128_t prod = (uint128_t)a * a;
            seq_t q = (seq_t)(prod / b);
            if ((uint128_t)q * b != prod) ++q;  /* ceil */
            term_cache[i] = q - 1;
# else
            /* Fall back to 64‑bit arithmetic (sufficient for 8/16/32‑bit types). */
            unsigned long long prod = (unsigned long long)a * a;
            seq_t q = prod / b;
            if (prod % b != 0) ++q;          /* ceil */
            term_cache[i] = q - 1;
# endif
#endif
        }
    }
    cache_size = (size_t)n + 1;
}

/* --------------------------------------------------------------------- */
/*  Sequence function as required by the interface.                       */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return term_cache[(size_t)n];
}
