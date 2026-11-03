/*
 * A235110  Primes whose base-10 representation also represents a prime in
 *          base 13.
 * https://oeis.org/A235110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 687, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9948 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235110: primes whose decimal representation is also prime when read as a base‑13 number.
   The function builds the sequence incrementally using a sieve for decimal primes
   and testing each candidate's base‑13 interpretation for primality. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* convert a decimal prime to its base‑13 value ------------------------------ */
static seq_t _to_base13_from_decimal(idx_t p)
{
    seq_t val = 0;
    uint64_t x = (uint64_t)p;
    unsigned char digits[25];
    int dcnt = 0;

    if (x == 0) {
        digits[dcnt++] = 0;
    } else {
        while (x) {
            digits[dcnt++] = (unsigned char)(x % 10);
            x /= 10;
        }
    }
    for (int i = dcnt - 1; i >= 0; --i) {
        val = val * 13 + digits[i];
    }
    return val;
}

/* -------------------------------------------------------------------------- */
/* static state -------------------------------------------------------------- */
static idx_t       _next_candidate = 2;   /* next integer to test (prime)   */
static idx_t       _sieve_limit    = 0;   /* current sieve size             */
static uint8_t    *_is_composite   = NULL;/* sieve bitmap                   */

static seq_t      *_seq_cache      = NULL;/* stored sequence values         */
static size_t      _seq_len        = 0;    /* number of stored elements      */
static size_t      _seq_cap        = 0;    /* allocated capacity             */

/* -------------------------------------------------------------------------- */
/* expand the sieve to at least new_limit ----------------------------------- */
static void _expand_sieve(idx_t new_limit)
{
    if (new_limit <= _sieve_limit) return;

    /* free old table and allocate a fresh one */
    free(_is_composite);
    _is_composite = (uint8_t*)calloc((size_t)(new_limit + 1), 1);
    _sieve_limit  = new_limit;

    for (idx_t i = 2; (idx_t)i * i <= new_limit; ++i) {
        if (!_is_composite[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i) {
                _is_composite[j] = 1;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* append a value to the sequence cache ------------------------------------- */
static void _push_seq(seq_t val)
{
    if (_seq_len == _seq_cap) {
        size_t new_cap = (_seq_cap) ? _seq_cap * 2 : 4;
        _seq_cache = seq_realloc(_seq_cache, _seq_cap, new_cap);
        _seq_cap   = new_cap;
    }
    _seq_cache[_seq_len++] = val;
}

/* -------------------------------------------------------------------------- */
/* ensure that a(n) exists for all n <= desired ----------------------------- */
static void _ensure_seq_len(idx_t desired)
{
    while (_seq_len < (size_t)desired) {
        /* enlarge sieve if necessary */
        if (_next_candidate > _sieve_limit) {
            idx_t new_limit = (_sieve_limit == 0) ? 1000 : _sieve_limit * 2;
            if (new_limit < _next_candidate) new_limit = _next_candidate * 2;
            _expand_sieve(new_limit);
        }

        /* test the candidate */
        if (_next_candidate == 2 ||
            !_is_composite[_next_candidate]) {
            seq_t base13 = _to_base13_from_decimal(_next_candidate);
            if (is_prime_big(base13)) {
                _push_seq((seq_t)_next_candidate);
            }
        }

        /* advance to next candidate */
        if (_next_candidate == 2) _next_candidate = 3;
        else _next_candidate += 2;   /* skip even numbers */
    }
}

/* -------------------------------------------------------------------------- */
/* public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */
    _ensure_seq_len(n);
    return _seq_cache[n - 1];
}
