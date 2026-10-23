/*
 * A145488  Numbers k such that 6k+13 is prime and 12k+13 is also prime.
 * https://oeis.org/A145488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k (starting at k=0) such that * both
 *            6k+13 and 12k+13 are prime. Values are generated on demand
 * Range:     u8 n <= 46, u16 n <= 3849, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static size_t _capacity = 0;
static size_t _count   = 0;
static seq_t *_cache   = NULL;
static idx_t _next_k   = 0;

/* Ensure that the internal cache can hold at least `needed` entries.  */
static void _ensure_capacity(size_t needed)
{
    if (needed <= _capacity) return;
    size_t new_cap = _capacity ? _capacity << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    _cache   = newptr;
    _capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset starts at 1 */
    if (!_cache) {                           /* first call */
        _capacity = 16;
        _cache    = (seq_t *)calloc(_capacity, sizeof(seq_t));
    }
    while (_count < n) {                     /* generate until we have enough */
        idx_t k = _next_k;
        idx_t p6  = (k << 2) + (k << 1) + 13; /* 6k+13 */
        idx_t p12 = (k << 3) + (k << 2) + 13; /* 12k+13 */
        if (is_prime(p6) && is_prime(p12)) {
            _ensure_capacity(_count + 1);
            _cache[_count++] = (seq_t)k;
        }
        ++_next_k;
    }
    return _cache[n - 1];
}
