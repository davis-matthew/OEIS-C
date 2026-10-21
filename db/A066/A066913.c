/*
 * A066913  (sum of primes < n that do not divide n) (mod n).
 * https://oeis.org/A066913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum of primes p < n with n mod p ≠ 0) mod n (OEIS
 *            A066913)
 * Range:     u8 n <= 43, u16 n <= 965, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- prime list handling ------------------------------------- */
static idx_t   _prime_max_generated = 0;  /* largest number checked for primes */
static idx_t * _prime_list          = NULL;  /* dynamic array of primes */
static size_t  _prime_count         = 0;     /* number of primes stored */
static size_t  _prime_cap           = 0;     /* allocated capacity */

/* Grow the prime array to the requested capacity */
static void _grow_primes(size_t new_cap)
{
    idx_t *tmp = (idx_t *)realloc(_prime_list, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    _prime_list = tmp;
    _prime_cap  = new_cap;
}

/* Ensure all primes less than 'limit' are stored */
static void _ensure_primes_up_to(idx_t limit)
{
    if (limit <= 2) return;          /* nothing to generate */

    /* initialise on first call */
    if (_prime_count == 0) {
        _grow_primes(128);
        _prime_list[0] = 2;
        _prime_count   = 1;
        _prime_max_generated = 2;
    }

    for (idx_t cand = _prime_max_generated + 1; cand < limit; ++cand) {
        if ((cand & 1) == 0) continue; /* skip even numbers */
        bool is_p = true;
        for (size_t i = 0; i < _prime_count; ++i) {
            idx_t p = _prime_list[i];
            if (p > cand / p) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (_prime_count == _prime_cap) _grow_primes(_prime_cap * 2);
            _prime_list[_prime_count++] = cand;
        }
    }
    _prime_max_generated = limit - 1;
}

/* ---------- cache for sequence values ------------------------------- */
static seq_t * _cache     = NULL;  /* _cache[k] stores a(k) for 0 <= k */
static size_t  _cache_cap = 0;     /* allocated capacity */
static idx_t   _cache_len = 0;     /* highest index stored in cache */

/* Grow the cache array to the requested capacity */
static void _grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    _cache = tmp;
    _cache_cap = new_cap;
}

/* Compute a(k) for all k up to 'n' and store them in the cache */
static void _ensure_cache_up_to(idx_t n)
{
    if (n <= _cache_len) return;

    if (_cache_cap < (size_t)(n + 1)) {
        size_t new_cap = _cache_cap ? _cache_cap * 2 : 128;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        _grow_cache(new_cap);
    }

    /* Make sure primes are available for the new range */
    _ensure_primes_up_to(n);

    for (idx_t k = _cache_len + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (size_t i = 0; i < _prime_count && _prime_list[i] < k; ++i) {
            idx_t p = _prime_list[i];
            if (k % p != 0) sum += p;
        }
        _cache[k] = sum % (seq_t)k;
    }
    _cache_len = n;
}

/* ---------- sequence accessor --------------------------------------- */
extern seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    _ensure_cache_up_to(n);
    return _cache[n];
}
