/*
 * A135141  a(1)=1, a(p_n)=2*a(n), a(c_n)=2*a(n)+1, where p_n = n-th prime,
 *          c_n = n-th composite number.
 * https://oeis.org/A135141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=1; a(p_n)=2*a(n); a(c_n)=2*a(n)+1, where p_n is n‑th prime,
 *            c_n is n‑th composite number
 * Range:     u8 n <= 59, u16 n <= 493, u32 n <= 6928, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static size_t _cache_limit = 0;
static bool * _is_prime = NULL;
static idx_t * _prime_pi = NULL;
static seq_t * _a_cache = NULL;

/* Sieve of Eratosthenes up to _cache_limit, fill _is_prime and _prime_pi */
static void _build_sieve(size_t limit) {
    _is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    _prime_pi = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    _a_cache   = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!_is_prime || !_prime_pi || !_a_cache) return;
    for (size_t i = 0; i <= limit; ++i) _is_prime[i] = true;
    _is_prime[0] = _is_prime[1] = false;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (_is_prime[p]) {
            for (size_t x = p * p; x <= limit; x += p) _is_prime[x] = false;
        }
    }
    _prime_pi[0] = 0;
    for (size_t i = 1; i <= limit; ++i) {
        _prime_pi[i] = _prime_pi[i-1] + (_is_prime[i] ? 1 : 0);
    }
    /* compute a[1] */
    _a_cache[1] = (seq_t)1;
    for (size_t n = 2; n <= limit; ++n) {
        if (_is_prime[n]) {
            idx_t rank = _prime_pi[n];          /* p_n */
            _a_cache[n] = (seq_t)2 * _a_cache[rank];
        } else {
            idx_t rank = (idx_t)n - _prime_pi[n] - (idx_t)1; /* c_n */
            _a_cache[n] = (seq_t)2 * _a_cache[rank] + (seq_t)1;
        }
    }
    _cache_limit = limit;
}

static void _ensure_limit(idx_t n) {
    if ((size_t)n <= _cache_limit) return;
    size_t new_limit = (size_t)n;
    /* extend arrays by rebuilding up to new_limit */
    free(_is_prime);
    free(_prime_pi);
    free(_a_cache);
    _build_sieve(new_limit);
}

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;          /* undefined for n<1 */
    _ensure_limit(n);
    return _a_cache[(size_t)n];
}
