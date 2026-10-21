/*
 * A069262  a(n) = 4*prime(n)^2.
 * https://oeis.org/A069262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*prime(n)^2 (OEIS A069262)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"

/* Compute the n-th prime using a simple sieve of Eratosthenes.
 * The primes are cached statically so consecutive calls are fast.
 */

static bool _primes_generated = false;
static uint32_t *_primes = NULL;
static size_t _primes_cnt = 0;
static size_t _primes_cap = 0;

static void _generate_primes(idx_t up_to_n) {
    if (_primes_generated) return;
    size_t limit = _prime_limit_for_idx(up_to_n);
    /* Ensure limit at least 15 */
    if (limit < 15) limit = 15;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) return; /* allocation failed, shouldn't happen */

    size_t cap = 2000;
    _primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!_primes) { free(is_composite); return; }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* record prime */
            if (cnt == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(_primes, cap * sizeof(uint32_t));
                if (!tmp) break;
                _primes = tmp;
            }
            _primes[cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    _primes_cnt = cnt;
    _primes_cap = cap;
    _primes_generated = true;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* behaviour for offset 0 irrelevant */
    /* ensure primes up to at least n */
    if (!_primes_generated || _primes_cnt < (size_t)n) {
        _generate_primes(n);
        /* if still not enough (unlikely), re-extend */
        if (_primes_cnt < (size_t)n) {
            /* Estimate larger bound */
            size_t new_limit = _prime_limit_for_idx(n) * 2;
            /* Re-generate with larger limit */
            _primes_generated = false;
            if (_primes) { free(_primes); _primes = NULL; }
            _primes_cnt = 0;
            _primes_cap = 0;
            _generate_primes((idx_t)new_limit);
        }
    }
    uint32_t prime_val = _primes[n - 1];
    seq_t p = (seq_t)prime_val;
    return (seq_t)4 * p * p;
}
