/*
 * A068544  Numbers k such that k*tau(k) > 2*prime(k).
 * https://oeis.org/A068544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Computes the n‑th integer k such that k*tau(k) > 2*prime(k) by a
 *            simple brute‑force search
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Types seq_t and idx_t are provided by the build. */
static bool _init_done = false;

#define SIEVE_LIMIT 6000000u    /* limit for sieve and spf arrays */

/* Sieve data */
static uint32_t * _spf          = NULL;   /* smallest prime factor */
static bool     * _is_prime     = NULL;   /* primality flag */
static uint32_t * _prime_list   = NULL;   /* list of primes (1‑based) */
static size_t     _prime_cnt    = 0;

/* Sequence storage */
static seq_t * _seq         = NULL;
static size_t  _seq_len     = 0;
static size_t  _seq_cap     = 0;

/* Last integer tested */
static idx_t _last_checked = 0;

/* ------------------------------------------------------------------ */
/* Push value to the dynamic sequence array                           */
/* ------------------------------------------------------------------ */
static void _seq_push(seq_t v)
{
    if (_seq_len == _seq_cap) {
        _seq_cap = _seq_cap ? (_seq_cap << 1) : 8;
        _seq = (seq_t *)realloc(_seq, _seq_cap * sizeof(seq_t));
    }
    _seq[_seq_len++] = v;
}

/* ------------------------------------------------------------------ */
/* Compute tau(k) using the spf array                                 */
/* ------------------------------------------------------------------ */
static uint32_t _tau_of(uint32_t k)
{
    if (k <= 1u) return 1u;
    uint32_t result = 1u;
    while (k > 1u) {
        uint32_t p = _spf[k];
        uint32_t e = 0u;
        while (k % p == 0u) {
            k /= p;
            ++e;
        }
        result *= (e + 1u);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Initialise sieve, spf array and prime list                         */
/* ------------------------------------------------------------------ */
static void _init(void)
{
    if (_init_done) return;
    _init_done = true;

    /* Allocate arrays */
    _spf      = (uint32_t *)calloc((size_t)SIEVE_LIMIT + 1, sizeof(uint32_t));
    _is_prime = (bool *)calloc((size_t)SIEVE_LIMIT + 1, sizeof(bool));

    _is_prime[0] = _is_prime[1] = false;

    /* Sieve of Eratosthenes with SPF */
    for (uint32_t i = 2u; i <= SIEVE_LIMIT; ++i) {
        if (_spf[i] == 0u) {            /* i is prime */
            _spf[i] = i;
            _is_prime[i] = true;
            if (i <= SIEVE_LIMIT / i) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    if (_spf[j] == 0u) _spf[j] = i;
                }
            }
        } else {
            _is_prime[i] = false;
        }
    }

    /* Build prime list */
    size_t cap = 400000;              /* enough for ~384 k primes up to 6M */
    _prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    _prime_cnt = 0;
    for (uint32_t i = 2u; i <= SIEVE_LIMIT; ++i) {
        if (_is_prime[i]) {
            if (_prime_cnt == cap) {    /* grow if necessary */
                cap <<= 1;
                _prime_list = (uint32_t *)realloc(_prime_list, cap * sizeof(uint32_t));
            }
            _prime_list[_prime_cnt++] = i;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main function required by the build system                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset */

    _init();

    /* Extend the sequence until it contains at least n terms. */
    while (_seq_len < (size_t)n) {
        ++_last_checked;  /* next k to test */

        /* Safety guard – should never be reached for n up to 10 000. */
        if (_last_checked > SIEVE_LIMIT) {
            return (seq_t)0;
        }

        uint32_t k   = (uint32_t)_last_checked;
        uint32_t t   = _tau_of(k);
        seq_t left   = (seq_t)_last_checked * (seq_t)t;
        seq_t right  = (seq_t)2 * (seq_t)_prime_list[_last_checked - 1]; /* 1‑based */

        if (left > right) {
            _seq_push((seq_t)k);
        }
    }

    return _seq[(size_t)n - 1];
}
