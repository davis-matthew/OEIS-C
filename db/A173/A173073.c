/*
 * A173073  (n-th nonnegative nonprime) minus (n-1).
 * https://oeis.org/A173073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑th nonnegative nonprime) – (n‑1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache for nonprime numbers. */
static idx_t  _cache_next_i   = 0;      /* next integer to test          */
static idx_t  _cache_noncnt   = 0;      /* number of nonprimes cached   */
static seq_t *_cache_mem      = NULL;   /* cache buffer                  */
static size_t _cache_cap      = 0;      /* current capacity of the cache */

/* Ensure that the cache can hold at least 'needed' elements. */
static void
_cache_ensure_cap(size_t needed)
{
    if (_cache_cap >= needed)
        return;
    size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_mem = (seq_t *)realloc(_cache_mem, new_cap * sizeof(seq_t));
    if (!new_mem)
        abort();      /* No memory – terminate */
    _cache_mem = new_mem;
    _cache_cap = new_cap;
}

/* Fill the cache until it contains at least 'n' nonprime numbers. */
static seq_t
_compute_nth_nonprime(idx_t n)
{
    while (_cache_noncnt < n) {
        if (!is_prime(_cache_next_i)) {
            _cache_noncnt++;
            _cache_ensure_cap(_cache_noncnt);
            _cache_mem[_cache_noncnt - 1] = (seq_t)_cache_next_i;
        }
        _cache_next_i++;
    }
    return _cache_mem[n - 1];
}

seq_t
a(idx_t n)
{
    if (n == 0)                /* offset is 1; n=0 is undefined – return 0 */
        return (seq_t)0;
    seq_t nth_nonprime = _compute_nth_nonprime(n);
    return nth_nonprime - (seq_t)(n - 1);
}
