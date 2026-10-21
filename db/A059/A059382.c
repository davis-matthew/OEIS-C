/*
 * A059382  a(n) = Product_{i=1..n} J_3(i).
 * https://oeis.org/A059382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} J_3(i) where J_3(i) = i^3 ∏_{p|i} (1 - 1/p^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   _build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A059382.
 * The function is memoised: repeated or out‑of‑order calls are fast.
 * It uses a smallest‑prime‑factor sieve for factoring and an integer
 * product for the Jordan totient values.  All arithmetic is done with
 * the build‑dependent type seq_t.  No floating point or external
 * libraries are used. */

#include <stdlib.h>   /* malloc, calloc, realloc, free */
#include "../utils/_build_spf.h"

static idx_t _spf_limit = 0;
static idx_t *_spf = NULL;           /* _spf[0] unused, _spf[1] = 0    */

/* Make sure the SPF table covers at least “limit”.  Returns the
 * final limit reached (may be greater than the requested limit). */
static idx_t _ensure_spf(idx_t limit)
{
    if (limit <= _spf_limit) return _spf_limit;
    idx_t *new_spf = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!new_spf) return _spf_limit;          /* allocation failed – keep old */
    if (_spf) free(_spf);
    _spf = new_spf;
    _build_spf(limit, _spf);
    _spf_limit = limit;
    return _spf_limit;
}

/* Cache for a(i) values.  Indexes 1.._last are always valid. */
static idx_t   _cache_cap = 0;
static seq_t *_cache      = NULL;
static idx_t   _last      = 0;        /* largest n whose a(n) is cached */

static void _ensure_cache(idx_t n)
{
    if (n + 1 <= _cache_cap) return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                   /* allocation failed – keep old */
    _cache = tmp;
    _cache_cap = new_cap;
}

/* Compute a(n) = ∏_{i=1..n} J_3(i). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    /* Ensure data structures are large enough. */
    _ensure_spf(n);
    _ensure_cache(n);
    if (_last == 0) _cache[0] = 1;       /* a(0) not used but convenient */

    /* Compute missing terms. */
    for (idx_t i = _last + 1; i <= n; ++i) {
        /* Compute J_3(i).  Start with J = 1. */
        seq_t J = 1;
        idx_t x = i;
        while (x > 1) {
            idx_t p = _spf[x];
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            /* Multiply by p^(3e). */
            seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
            for (idx_t t = 0; t < e; ++t) J *= p3;

            /* Multiply by (p^3 - 1) / p^3. */
            J = J * (p3 - 1);
            J = J / p3;
        }

        /* Accumulate the product. */
        if (i == 1) {
            _cache[1] = J;
        } else {
            _cache[i] = _cache[i-1] * J;
        }
    }

    _last = n;
    return _cache[n];
}
