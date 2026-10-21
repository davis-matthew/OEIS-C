/*
 * A209800  Numbers n such that the concatenation of the distinct prime
 *          divisors of n is composite.
 * https://oeis.org/A209800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1094, u32 n <= 1094, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, digit_len, int_sqrt, is_prime_big, pow10,
 *            seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation for sequence A209800
 * "Numbers n such that the concatenation of the distinct prime divisors
 * of n is composite."
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t) */
#include "../utils/pow10.h"            /* seq_t pow10(idx_t) */
#include "../utils/digit_len.h"        /* int digit_len(seq_t) */
#include "../utils/build_primes_upto.h"/* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */
#include "../utils/int_sqrt.h"        /* idx_t int_sqrt(idx_t) */
#include "../utils/seq_alloc.h"       /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_free.h"        /* void seq_free(seq_t *) */

static seq_t      *_terms      = NULL;   /* Cached terms of the sequence */
static size_t      _terms_cap  = 0;
static size_t      _terms_len  = 0;

static uint64_t   *_prime_list = NULL;   /* Array of primes used for factorisation */
static size_t      _prime_cnt  = 0;
static idx_t       _prime_limit = 0;    /* Largest prime currently generated */

static idx_t       _last_checked = 2;    /* Smallest n yet tested */

/* Ensure that all primes <= limit are available in _prime_list. */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= _prime_limit) return;

    if (_prime_list) seq_free((seq_t *)_prime_list);
    build_primes_upto(limit, &_prime_list, &_prime_cnt);
    _prime_limit = limit;
}

/* Append a value to the cached terms array, enlarging if necessary. */
static void _push_term(seq_t val)
{
    if (_terms_len == _terms_cap) {
        size_t new_cap = (_terms_cap == 0) ? 16 : _terms_cap * 2;
        seq_t *new_arr = seq_alloc(new_cap);
        memcpy(new_arr, _terms, _terms_len * sizeof(seq_t));
        if (_terms) seq_free((seq_t *)_terms);
        _terms      = new_arr;
        _terms_cap  = new_cap;
    }
    _terms[_terms_len++] = val;
}

/* Test whether integer m satisfies the sequence condition.  Return true
 * if the concatenation of its distinct prime divisors is composite. */
static bool _check_candidate(uint64_t m)
{
    if (m < 6) return false;               /* need at least two distinct primes */

    uint64_t temp = m;
    uint64_t primes[64];
    size_t   pcnt  = 0;

    /* Ensure the prime list covers all possible divisors of m. */
    idx_t sqrt_m = int_sqrt((idx_t)m);
    _ensure_primes_upto(sqrt_m);

    /* Trial division using the prime list. */
    for (size_t i = 0; i < _prime_cnt && _prime_list[i] <= temp / _prime_list[i]; ++i) {
        uint64_t p = _prime_list[i];
        if (temp % p == 0) {
            primes[pcnt++] = p;
            do { temp /= p; } while (temp % p == 0);
        }
    }
    if (temp > 1) primes[pcnt++] = temp;   /* remainder is a prime factor */

    if (pcnt <= 1) return false;           /* need at least two distinct primes */

    /* Build the concatenation of the distinct primes. */
    seq_t concat = (seq_t)primes[0];
    for (size_t i = 1; i < pcnt; ++i) {
        seq_t pow_val = pow10((idx_t)digit_len((seq_t)primes[i]));
        concat = concat * pow_val + (seq_t)primes[i];
    }

    /* The concatenation must be composite. */
    return !is_prime_big(concat);
}

/* Maximum size reserved for the cache (sufficient for n <= 10000). */
#define MAX_CACHE 20000

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0, return 0 */

    /* Initialise the cache with a fixed large capacity to avoid realloc. */
    if (_terms_cap == 0) {
        _terms_cap = MAX_CACHE;
        _terms = seq_alloc(_terms_cap);
    }

    while (_terms_len < (size_t)n) {
        if (_check_candidate(_last_checked))
            _push_term((seq_t)_last_checked);
        ++_last_checked;
    }

    return _terms[n - 1];
}
