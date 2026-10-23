/*
 * A036333  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 9.
 * https://oeis.org/A036333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_len_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: brute‑force search of composite numbers whose concatenated
   prime factors (counted with multiplicity) have exactly 9 decimal
   digits.  The results are cached for subsequent calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/_digit_len_uint64.h"

/* ---------- simple prime sieve up to 1,000,000 (enough for sqrt) */
#define PRIME_SIEVE_LIMIT 1000000
static uint32_t _primes[120000];   /* 1000000 has 78498 primes */
static size_t   _prime_cnt = 0;
static bool     _primes_ready = false;

static void _generate_primes(void)
{
    if (_primes_ready) return;
    bool *is_composite = (bool*)malloc(sizeof(bool) * (PRIME_SIEVE_LIMIT + 1));
    if (!is_composite) exit(EXIT_FAILURE);
    memset(is_composite, 0, sizeof(bool) * (PRIME_SIEVE_LIMIT + 1));

    for (int i = 2; (int64_t)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_composite[j] = true;
        }
    }
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            _primes[_prime_cnt++] = (uint32_t)i;
        }
    }
    free(is_composite);
    _primes_ready = true;
}

/* ---------- cache for sequence values -------------------------- */
static seq_t * _cache  = NULL;   /* 0‑based array of terms */
static size_t  _cache_cap = 0;
static idx_t   _cache_len = 0;
static uint64_t _next_candidate = 4;   /* start testing from 4 */

/* ensure cache has room for 'need' elements (need > current cap) */
static void _ensure_cache_cap(idx_t need)
{
    if ((size_t)need <= _cache_cap) return;
    size_t ncap = _cache_cap == 0 ? 8 : _cache_cap * 2;
    while (ncap < (size_t)need) ncap *= 2;
    seq_t *newp = (seq_t*)realloc(_cache, ncap * sizeof(seq_t));
    if (!newp) exit(EXIT_FAILURE);
    _cache = newp;
    _cache_cap = ncap;
}

/* ---------- main sequence function ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; 0 is unused */
    _generate_primes();

    if ((idx_t)n <= _cache_len)      /* already computed */
        return _cache[n - 1];

    _ensure_cache_cap(n);

    /* generate terms until we have n of them */
    while (_cache_len < n) {
        uint64_t num = _next_candidate++;
        uint64_t temp = num;
        int digits = 0;
        bool composite = false;

        for (size_t i = 0; i < _prime_cnt; ++i) {
            uint32_t p = _primes[i];
            uint64_t pp = (uint64_t)p * p;
            if (pp > temp) break;
            if (temp % p == 0) {
                composite = true;
                while (temp % p == 0) {
                    digits += _digit_len_uint64(p);
                    temp /= p;
                }
            }
        }

        if (temp > 1) { /* remaining prime factor > sqrt(num) */
            if (composite) {
                digits += _digit_len_uint64(temp);
            } else {
                /* temp == num and no smaller factor found => num is prime */
                composite = false;
            }
        }

        if (composite && digits == 9) {
            _cache[_cache_len++] = (seq_t)num;
        }
    }

    return _cache[n - 1];
}
