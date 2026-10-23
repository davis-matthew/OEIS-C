/*
 * A104213  Primes with nonprime sums of digits.
 * https://oeis.org/A104213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 3741, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_sum_prime_small, _digit_sum_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A104213:  primes whose digit sum is not prime
   by generating primes incrementally and filtering them.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_digit_sum_prime_small.h"
#include "../utils/_digit_sum_uint64.h"

/* ---------- dynamic array for primes (uint64_t) ---------- */
static uint64_t * _prime_list = NULL;
static size_t     _prime_cnt  = 0;
static size_t     _prime_cap  = 0;

static void _ensure_prime_cap(size_t needed)
{
    if (_prime_cap >= needed) return;
    size_t new_cap = _prime_cap ? _prime_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *new_arr = (uint64_t *)realloc(_prime_list, new_cap * sizeof(uint64_t));
    _prime_list = new_arr;
    _prime_cap  = new_cap;
}

/* Test whether n is prime using the current _prime_list */
static bool _is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate next prime and append to _prime_list */
static uint64_t _next_prime(void)
{
    static uint64_t candidate = 1;
    for (;;) {
        ++candidate;
        if (_is_prime_uint64(candidate)) {
            _ensure_prime_cap(_prime_cnt + 1);
            _prime_list[_prime_cnt++] = candidate;
            return candidate;
        }
    }
}

/* ---------- cache for the sequence ---------- */
static seq_t * _seq_cache = NULL;
static idx_t  _cache_size = 0;
static idx_t  _cache_cap  = 0;

static void _ensure_seq_cache(idx_t needed)
{
    if (_cache_cap >= needed) return;
    idx_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(_seq_cache, new_cap * sizeof(seq_t));
    _seq_cache = new_arr;
    _cache_cap = new_cap;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */
    /* Extend the cache until it contains at least n terms */
    if (_cache_size < n) {
        _ensure_seq_cache(n);
        while (_cache_size < n) {
            uint64_t p = _next_prime();
            int s = _digit_sum_uint64(p);
            if (!_digit_sum_prime_small(s)) {
                _seq_cache[_cache_size++] = (seq_t)p;
            }
        }
    }
    return _seq_cache[n - 1];
}
