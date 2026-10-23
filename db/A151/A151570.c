/*
 * A151570  a(0)=1, a(1)=1; a(2^i + j) = a(j) + a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1014, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS recurrence:
   a(0) = 1, a(1) = 1;
   for n ≥ 2 let i = ⌊log₂(n)⌋, p = 2ⁱ, j = n - p.
   Then a(n) = a(j) + a(j + 1)   (0 ≤ j < p) */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Small helper: floor(log₂(v)) for v ≥ 1 */
static idx_t _floor_log2(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

#ifdef __cplusplus
/* C++ implementation uses std::vector for the memoisation cache */
static std::vector<seq_t> _cache;   /* values, 0 means not yet computed */

/* Ensure _cache has space for indices up to n */
static void _ensure_cache(idx_t n)
{
    if (_cache.size() <= (size_t)n)
        _cache.resize((size_t)n + 1, seq_t(0));      /* initialise new slots to 0 */
}
#else
/* C implementation – raw memory allocation */
static seq_t * _cache = NULL;     /* array of values */
static size_t _cache_cap = 0;     /* allocated capacity */

/* Ensure _cache has space up to index n, zero‑initialise new part */
static void _ensure_cache(idx_t n)
{
    if (_cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (new_cap > _cache_cap) {
            /* Zero‑initialise the new tail – seq_t is assumed to be POD */
            memset(tmp + _cache_cap, 0, (new_cap - _cache_cap) * sizeof(seq_t));
        }
        _cache = tmp;
        _cache_cap = new_cap;
    }
}
#endif

/* Recursive computation with memoisation */
static seq_t _compute_a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(0) = a(1) = 1 */

    _ensure_cache(n);
#ifdef __cplusplus
    if (_cache[(size_t)n] != 0) return _cache[(size_t)n];
#else
    if (_cache[(size_t)n] != 0) return _cache[(size_t)n];
#endif

    idx_t i  = _floor_log2(n);          /* i = ⌊log₂(n)⌋ ≥ 1 */
    idx_t p  = (idx_t)1 << i;          /* p = 2ⁱ */
    idx_t j  = n - p;                  /* 0 ≤ j < p */

    seq_t res = _compute_a(j) + _compute_a(j + 1);

#ifdef __cplusplus
    _cache[(size_t)n] = res;
#else
    _cache[(size_t)n] = res;
#endif

    return res;
}

/* Public interface expected by the build system */
seq_t a(idx_t n)
{
    return _compute_a(n);
}
