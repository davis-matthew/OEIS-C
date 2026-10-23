/*
 * A049590  Primes p such that x^58 = 2 has a solution mod p.
 * https://oeis.org/A049590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that x^58 = 2 has a solution mod p (i.e. 2 is a
 *            58‑th power residue)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t x) */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/_gcd64.h"

static size_t _cache_cap   = 0;
static size_t _cache_count = 0;
static seq_t * _cache      = NULL;

/* Euclidean GCD for 64‑bit integers */

/* Ensure the cache has room for at least 'needed' elements */
static void _ensure_cap(size_t needed)
{
    if (needed <= _cache_cap)
        return;
    size_t new_cap = _cache_cap < 8 ? 8 : _cache_cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    _cache      = tmp;
    _cache_cap  = new_cap;
}

/* Return the n-th prime p with 2 a 58‑th power residue mod p (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* behaviour below offset is undefined */

    /* start searching from the first prime not yet examined */
    uint64_t candidate = (_cache_count == 0) ? 2
                                            : (uint64_t)(_cache[_cache_count - 1]) + 1;

    while (_cache_count < (size_t)n) {
        if (is_prime((idx_t)candidate)) {
            if (candidate == 2) {             /* special‑case p = 2 */
                _ensure_cap(_cache_count + 1);
                _cache[_cache_count++] = (seq_t)candidate;
            } else {
                uint64_t g    = _gcd64(58ULL, candidate - 1);
                uint64_t exp  = (candidate - 1) / g;
                if (mod_pow((seq_t)2, exp, (seq_t)candidate) == (seq_t)1) {
                    _ensure_cap(_cache_count + 1);
                    _cache[_cache_count++] = (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }

    return _cache[(size_t)n - 1];
}
