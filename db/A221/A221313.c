/*
 * A221313  Square root of number of nX4 arrays of occupancy after each
 *          element moves to some horizontal or vertical neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) Initial
 *            values: a(1)=1, a(2)=4, a(3)=9, a(4)=23, a(5)=57
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> _a_cache;

static void _ensure_cache(idx_t n)
{
    if (_a_cache.size() >= n)
        return;
    _a_cache.reserve(n);
    while (_a_cache.size() < n) {
        idx_t i = _a_cache.size() + 1;  // current index (1‑based)
        seq_t val;
        if (i == 1)           val = 1;
        else if (i == 2)      val = 4;
        else if (i == 3)      val = 9;
        else if (i == 4)      val = 23;
        else if (i == 5)      val = 57;
        else {
            /* a(n) = 2*a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)-a(n-5) */
            val = 2 * _a_cache[i-2] + 2 * _a_cache[i-3];
            val -= _a_cache[i-4];
            val -= 2 * _a_cache[i-5];
            val -= _a_cache[i-6];
        }
        _a_cache.push_back(val);
    }
}
#else  /* C implementation */
#include <stdlib.h>
static seq_t * _a_cache = NULL;
static idx_t _a_cache_len = 0;   /* number of terms stored (a(1) at index 0) */
static idx_t _a_cache_cap = 0;   /* allocated capacity */

static void _ensure_cache(idx_t n)
{
    if (_a_cache_len >= n)
        return;
    /* grow capacity if needed */
    if (_a_cache_cap < n) {
        idx_t new_cap = _a_cache_cap ? _a_cache_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(_a_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failure */
        _a_cache      = tmp;
        _a_cache_cap  = new_cap;
    }
    /* compute missing terms */
    for (idx_t i = _a_cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)           val = 1;
        else if (i == 2)      val = 4;
        else if (i == 3)      val = 9;
        else if (i == 4)      val = 23;
        else if (i == 5)      val = 57;
        else {
            /* a(n) = 2*a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)-a(n-5) */
            val = 2 * _a_cache[i-2] + 2 * _a_cache[i-3];
            val -= _a_cache[i-4];
            val -= 2 * _a_cache[i-5];
            val -= _a_cache[i-6];
        }
        _a_cache[i-1] = val;
    }
    _a_cache_len = n;
}
#endif

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined, but return 0 for safety */
    _ensure_cache(n);
#ifdef __cplusplus
    return _a_cache[n-1];
#else
    return _a_cache[n-1];
#endif
}
