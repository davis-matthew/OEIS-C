/*
 * A209800  Numbers n such that the concatenation of the distinct prime
 *          divisors of n is composite.
 * https://oeis.org/A209800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9884 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, isqrt, mem, num_digits
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

#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
    return mem_new(seq_t, cap);
}

static void seq_free(seq_t *p) { mem_free(p); }

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

    free(_prime_list);
    build_primes_upto(limit, &_prime_list, &_prime_cnt);
    _prime_limit = limit;
}

/* Append a value to the cached terms array, enlarging if necessary. */
static void _push_term(seq_t val)
{
    if (_terms_len == _terms_cap) {
        size_t new_cap = (_terms_cap == 0) ? 16 : _terms_cap * 2;
        seq_t *new_arr = mem_resize(seq_t, _terms, new_cap);
        if (!new_arr) abort();
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
    idx_t sqrt_m = isqrt((idx_t)m);
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

    /* Build the concatenation of the distinct primes: in 64 bits, tested
       exactly, when it fits (seq_t arithmetic overflows in the squaring of
       Miller-Rabin once it passes 2^32), else in seq_t. */
    uint64_t c64 = primes[0];
    bool fits = true;
    for (size_t i = 1; i < pcnt && fits; ++i) {
        uint64_t pw = 1;
        for (uint64_t q = primes[i]; q; q /= 10) pw *= 10;
        fits = !__builtin_mul_overflow(c64, pw, &c64) && !__builtin_add_overflow(c64, primes[i], &c64);
    }
    if (fits) return !is_prime_u64(c64);
    seq_t concat = (seq_t)primes[0];
    for (size_t i = 1; i < pcnt; ++i) {
        seq_t pow_val = pow10((idx_t)num_digits((seq_t)primes[i]));
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
