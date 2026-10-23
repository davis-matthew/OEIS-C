/*
 * A175703  Convolution of A007947 with itself.
 * https://oeis.org/A175703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} rad(k) * rad(n+1-k) where rad is A007947
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_spf, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/seq_alloc_array.h"

/* Smallest prime factor table: spf[i] gives the least prime dividing i.
   The table is allocated up to index _spf_cap-1. */
static idx_t _spf_cap   = 0;
static idx_t* _spf_cache = NULL;

/* radCache[i] = rad(i).  The table holds indices [0.._rad_cap-1]. */
static idx_t _rad_cap   = 0;
static seq_t* _rad_cache = NULL;

/* Ensure that _spf_cache contains values for indices up to at least limit. */
static void _ensure_spf(idx_t limit)
{
    if (limit + 1 <= _spf_cap)
        return;
    if (_spf_cache != NULL) {
        free(_spf_cache);
        _spf_cache = NULL;
    }
    /* build_spf allocates an array of size limit+1 and fills it. */
    build_spf(limit, &_spf_cache);
    _spf_cap = limit + 1;
}

/* Ensure that _rad_cache contains rad values for indices up to at least limit. */
static void _ensure_rad(idx_t limit)
{
    if (limit + 1 <= _rad_cap)
        return;

    idx_t new_cap = limit + 1;
    if (_rad_cache != NULL) {
        free(_rad_cache);
        _rad_cache = NULL;
    }
    _rad_cache = seq_alloc_array(new_cap);
    _rad_cache[0] = 0;          /* unused */
    _rad_cache[1] = 1;          /* rad(1) = 1 */

    for (idx_t i = 2; i <= limit; ++i) {
        /* least prime factor of i */
        uint64_t p64 = _spf_cache[i];
        seq_t p  = (seq_t)p64;
        idx_t m = i / (idx_t)p64;
        if (m % (idx_t)p64 == 0)
            _rad_cache[i] = _rad_cache[m];
        else
            _rad_cache[i] = _rad_cache[m] * p;
    }
    _rad_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    _ensure_spf(n);
    _ensure_rad(n);

    seq_t sum = 0;
    idx_t total = n + 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t left  = _rad_cache[k];
        seq_t right = _rad_cache[total - k];
        sum += left * right;
    }
    return sum;
}
