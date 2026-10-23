/*
 * A049944  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A049944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition of sequence A049944
   a(1)=1, a(2)=1, a(3)=4
   a(n)=sum_{i=1}^{n-1} a(i) + a(m)  for n>=4
   where m = 2*n - 3 - 2^{p+1}
   and p is the unique integer such that 2^p < n-1 <= 2^{p+1}
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Function prototype (should already exist) */
seq_t a(idx_t n);

/* ------------------------------------------------------------
   Helpers
   ------------------------------------------------------------ */

/* Integer floor‑log₂ for idx_t (n > 0) */
static inline idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* ------------------------------------------------------------
   Cache allocation and bookkeeping
   ------------------------------------------------------------ */
static idx_t max_computed = 0;   /* highest n we have computed yet */

#ifdef __cplusplus
/* C++ implementation uses std::vector */
static std::vector<seq_t> cache_vec;
static std::vector<seq_t> pref_vec;

static void ensure_cache(idx_t n)         /* ensure capacity for 0..n */
{
    if (cache_vec.size() <= (size_t)n) {
        cache_vec.resize((size_t)n + 1);
        pref_vec.resize((size_t)n + 1);
    }
}
#else
/* C implementation uses malloc/realloc */
static seq_t *cache  = NULL;
static seq_t *pref   = NULL;
static size_t cache_cap = 0;          /* actual capacity, count of indices */

static void ensure_cache(idx_t n)         /* ensure capacity for 0..n */
{
    if ((size_t)n + 1 <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)n + 1) new_cap <<= 1;

    seq_t *c = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    seq_t *p = (seq_t *)realloc(pref,  new_cap * sizeof(seq_t));
    if (!c || !p) abort();   /* out of memory -> hard fail */
    cache = c;
    pref  = p;
    cache_cap = new_cap;
}
#endif

/* Initialisation of the first four values */
static void init_sequence(void)
{
    if (max_computed >= 3) return;          /* already done */

    /* Ensure we have space for indices 0..3 */
    ensure_cache(3);

#ifdef __cplusplus
    if (cache_vec.size() < 4) cache_vec.resize(4);
    if (pref_vec.size() < 4)  pref_vec.resize(4);
    cache_vec[0] = 0; pref_vec[0] = 0;
    cache_vec[1] = 1; pref_vec[1] = 1;
    cache_vec[2] = 1; pref_vec[2] = 2;
    cache_vec[3] = 4; pref_vec[3] = 6;
#else
    cache[0] = 0; pref[0] = 0;
    cache[1] = 1; pref[1] = 1;
    cache[2] = 1; pref[2] = 2;
    cache[3] = 4; pref[3] = 6;
#endif

    max_computed = 3;
}

/* ------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range – harmless */
    init_sequence();          /* guarantees indices 1..3 exist */

    if (n <= max_computed) {
#ifdef __cplusplus
        return cache_vec[(size_t)n];
#else
        return cache[(size_t)n];
#endif
    }

    /* Grow cache to hold indices up to n */
    ensure_cache(n);

    /* Compute all missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* p = floor_log2(i-2)  (equals ceil(log2(i-1))-1) */
        idx_t p   = floor_log2_uint64(i - 2);
        idx_t pow = ((idx_t)1) << (p + 1);          /* 2^{p+1} */
        idx_t m   = 2 * i - 3 - pow;                /* always < i */

#ifdef __cplusplus
        seq_t val = pref_vec[(size_t)(i - 1)] + cache_vec[(size_t)m];
        cache_vec[(size_t)i] = val;
        pref_vec[(size_t)i]  = pref_vec[(size_t)(i - 1)] + val;
#else
        seq_t val = pref[(size_t)(i - 1)] + cache[(size_t)m];
        cache[(size_t)i] = val;
        pref[(size_t)i]  = pref[(size_t)(i - 1)] + val;
#endif
    }

    max_computed = n;      /* record highest computed index */

    /* Return the requested value */
#ifdef __cplusplus
    return cache_vec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
