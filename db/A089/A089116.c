/*
 * A089116  Convoluted convolved Fibonacci numbers G_j^(3).
 * https://oeis.org/A089116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=1, a(3)=3, a(4)=7, a(5)=17, a(6)=37,
   a(7)=77, a(8)=158, a(9)=314, a(10)=611, a(11)=1174, a(12)=2222.
   a(n) = 3*a(n-1) - 4*a(n-3) - 3*a(n-4) + 3*a(n-5) + 7*a(n-6) -
          3*a(n-7) - 3*a(n-8) + 4*a(n-9) - 3*a(n-11) - a(n-12)   (n ≥ 13) */

#include <stdlib.h>
#include <string.h>

static const seq_t _init[12] = {
    0, 1, 3, 7, 17, 37, 77, 158, 314, 611, 1174, 2222
};

static seq_t * _cache = NULL;
static idx_t    _cache_len = 0;

seq_t a(idx_t n)
{
    if (n <= 12) return _init[n-1];

    /* Ensure the cache holds the first 12 terms */
    if (_cache_len < 12) {
        _cache = (seq_t*)malloc(sizeof(seq_t)*12);
        if (!_cache) return 0;          /* allocation failure – return 0 */
        memcpy(_cache, _init, sizeof(_init));
        _cache_len = 12;
    }

    /* If the requested term is already cached, return it */
    if (n <= _cache_len) return _cache[n-1];

    /* Grow the cache to accommodate up to n */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(_cache, sizeof(seq_t)*new_len);
    if (!tmp) return 0;                /* allocation failure – return 0 */
    _cache = tmp;

    /* Compute terms from (cache_len+1) up to n */
    for (idx_t idx = _cache_len + 1; idx <= n; ++idx) {
        seq_t pos = 0, neg = 0;
        pos += 3 * _cache[(size_t)(idx-2)];
        pos += 3 * _cache[(size_t)(idx-6)];
        pos += 7 * _cache[(size_t)(idx-7)];
        pos += 4 * _cache[(size_t)(idx-10)];
        neg += 4 * _cache[(size_t)(idx-4)];
        neg += 3 * _cache[(size_t)(idx-5)];
        neg += 3 * _cache[(size_t)(idx-8)];
        neg += 3 * _cache[(size_t)(idx-9)];
        neg += 3 * _cache[(size_t)(idx-12)];
        neg +=    _cache[(size_t)(idx-13)];
        _cache[(size_t)(idx-1)] = pos - neg;
    }

    _cache_len = n;
    return _cache[n-1];
}
