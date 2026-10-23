/*
 * A037458  a(1)=1; for n > 1, a(n) = n - a(n-floor(sqrt(n))).
 * https://oeis.org/A037458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n - a(n - ⌊√n⌋) (a(1) = 1)
 * Range:     u8 n <= 508, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Integer square root: floor(sqrt(n)), works for idx_t up to 64‑bit. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)       /* mid*mid <= n   – avoids overflow   */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* C++ implementation ------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> _cache_vec;
static idx_t _computed_max_cpp = 0;

/* Ensure that _cache_vec[i] is defined for 1 <= i <= n.  */
static void _ensure_cache(idx_t n)
{
    if (_cache_vec.size() <= n)
        _cache_vec.resize(n + 1);         /* index 0 unused           */

    for (idx_t i = _computed_max_cpp + 1; i <= n; ++i) {
        if (i == 1) {
            _cache_vec[1] = (seq_t)1;
        } else {
            idx_t floor_sqrt = _isqrt_u64(i);
            idx_t diff      = i - floor_sqrt;
            _cache_vec[i] = (seq_t)i - _cache_vec[diff];
        }
    }
    _computed_max_cpp = n;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* below offset: undefined behaviour ok  */
        return (seq_t)0;
    _ensure_cache(n);
    return _cache_vec[n];
}

/* ------------------------------------------------------------------ */
/* C implementation --------------------------------------------------- */
#else
static seq_t * _cache_c = NULL;   /* 1‑based array, index 0 unused       */
static idx_t   _cache_cap_c = 0;   /* allocated array size                */
static idx_t   _computed_max_c = 0;

/* Ensure that _cache_c[i] is defined for 1 <= i <= n.  */
static void _ensure_cache(idx_t n)
{
    if (n < 1)
        return;

    if (n + 1 > _cache_cap_c) {
        idx_t new_cap = (n + 1) * 2;           /* grow factor 2           */
        seq_t *newp = (seq_t *)realloc(_cache_c,
                                       new_cap * sizeof(seq_t));
        if (!newp)                         /* allocation failure      */
            exit(EXIT_FAILURE);            /* abort: fatal error       */
        _cache_c     = newp;
        _cache_cap_c = new_cap;
    }

    for (idx_t i = _computed_max_c + 1; i <= n; ++i) {
        if (i == 1) {
            _cache_c[1] = (seq_t)1;
        } else {
            idx_t floor_sqrt = _isqrt_u64(i);
            idx_t diff      = i - floor_sqrt;
            _cache_c[i] = (seq_t)i - _cache_c[diff];
        }
    }
    _computed_max_c = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    _ensure_cache(n);
    return _cache_c[n];
}
#endif
