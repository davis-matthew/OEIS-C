/*
 * A235110  Primes whose base-10 representation also represents a prime in
 *          base 13.
 * https://oeis.org/A235110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 687, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _to_base13_from_decimal, is_prime_big, seq_realloc
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

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t) */
#include "../utils/_to_base13_from_decimal.h"

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
