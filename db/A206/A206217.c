/*
 * A206217  Number of (n+1)X3 0..2 arrays with rows and columns of
 *          determinants of all 2X2 subblocks lexicographically nondecreasing.
 * https://oeis.org/A206217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) -49*a(n-2) -97*a(n-3) +664*a(n-4) -980*a(n-5)
 *            +448*a(n-6) * Initial values: * a(1)=452, a(2)=7313, a(3)=73133,
 *            a(4)=656053, a(5)=5862927, * a(6)=53835891, a(7)=499752207,
 *            a(8)=4654712482, a(9)=43385661909, * a(10)=404522337676,
 *            a(11)=3772143906359, a(12)=35177251141094, * a(13)=3...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _a_start[16] = {
    452ULL, 7313ULL, 73133ULL, 656053ULL,
    5862927ULL, 53835891ULL, 499752207ULL,
    4654712482ULL, 43385661909ULL, 404522337676ULL,
    3772143906359ULL, 35177251141094ULL,
    328055398640436ULL, 3059417909281589ULL,
    28532069184985757ULL, 266090406288547230ULL
};

static seq_t * _cache = NULL;          /* cached values, 0‑based */
static idx_t   _cache_len = 0;          /* number of values stored */
static idx_t   _cache_cap = 0;          /* allocated capacity */

/* Ensure that the cache can hold at least 'need' elements. */
static void _ensure_cache_cap(idx_t need)
{
    if (_cache_cap < need) {
        idx_t new_cap = _cache_cap ? _cache_cap * 2 : need;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        _cache = tmp;
        _cache_cap = new_cap;
    }
}

/* Compute the sequence value a(n).  idx_t is unsigned 64‑bit. */
seq_t a(idx_t n)
{
    /* n starts at 1 */
    if (n <= 16ULL)
        return _a_start[n - 1ULL];

    /* Make sure the cache contains at least the first 16 terms. */
    if (_cache_len < 16ULL) {
        _ensure_cache_cap(16ULL);
        for (idx_t i = _cache_len; i < 16ULL; ++i)
            _cache[i] = _a_start[i];
        _cache_len = 16ULL;
    }

    /* Grow cache to hold up to n. */
    if (_cache_len < n)
        _ensure_cache_cap(n);

    /* Compute new terms up to n. */
    for (idx_t i = _cache_len; i < n; ++i) {
        /* i corresponds to a(i+1).  Recurrence uses indices i-1 .. i-6. */
        seq_t val = 15ULL * _cache[i - 1ULL]
                  - 49ULL * _cache[i - 2ULL]
                  - 97ULL * _cache[i - 3ULL]
                  + 664ULL * _cache[i - 4ULL]
                  - 980ULL * _cache[i - 5ULL]
                  + 448ULL * _cache[i - 6ULL];
        _cache[i] = val;
    }
    _cache_len = n;

    return _cache[n - 1ULL];
}
