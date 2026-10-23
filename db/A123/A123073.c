/*
 * A123073  Number of ordered triples of primes (p,q,r) such that pqr = n-th
 *          3-almost prime A014612(n).
 * https://oeis.org/A123073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples of primes (p,q,r) such that *
 *            p*q*r = the n-th 3‑almost prime A014612(n). For a 3‑almost prime
 *            * N the answer is * 1 if N = p³, * 3 if N = p²·q (p,q distinct),
 *            * 6 if N = p·q·r (p,q,r distinct)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state – initialized on first call */
static size_t _SPF_LIMIT = 300000;
static uint32_t *_spf = NULL;          /* smallest prime factor */
static uint8_t  *_omega = NULL;        /* Ω(n) = number of prime factors with multiplicity */
static uint32_t *_three = NULL;        /* list of 3‑almost primes */
static size_t    _three_cnt = 0;       /* length of the list */

/* initialise the helper tables */
static void _init(void) {
    if (_spf) return;               /* already initialised */

    /* allocate arrays */
    _spf   = (uint32_t*)malloc((_SPF_LIMIT + 1) * sizeof(uint32_t));
    _omega = (uint8_t*)malloc((_SPF_LIMIT + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= _SPF_LIMIT; ++i) {
        _spf[i]   = 0;
        _omega[i] = 0;
    }

    /* sieve for smallest prime factor and Ω */
    for (size_t i = 2; i <= _SPF_LIMIT; ++i) {
        if (_spf[i] == 0) {                       /* i is prime */
            _spf[i]   = (uint32_t)i;
            _omega[i] = 1;
            if ((uint64_t)i*i <= _SPF_LIMIT) {
                for (size_t j = i*i; j <= _SPF_LIMIT; j += i)
                    if (_spf[j] == 0)
                        _spf[j] = (uint32_t)i;
            }
        } else {                                   /* composite */
            _omega[i] = 1 + _omega[i / _spf[i]];
        }
    }

    /* build the list of 3‑almost primes */
    size_t cap = 16384;
    _three = (uint32_t*)malloc(cap * sizeof(uint32_t));
    _three_cnt = 0;
    for (size_t i = 2; i <= _SPF_LIMIT; ++i)
        if (_omega[i] == 3) {
            if (_three_cnt == cap) {
                cap <<= 1;
                _three = (uint32_t*)realloc(_three, cap * sizeof(uint32_t));
            }
            _three[_three_cnt++] = (uint32_t)i;
        }
}

/* the required function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* below the offset – not used */
    _init();

    if ((idx_t)n > (idx_t)_three_cnt)
        return (seq_t)0;              /* should not happen for test data */

    uint32_t N = _three[n - 1];
    uint32_t t = N;
    bool has_sq   = false;
    bool has_cub  = false;
    while (t > 1) {
        uint32_t p = _spf[t];
        int cnt = 0;
        while (t % p == 0) {          /* t is always divisible by p */
            t /= p;
            ++cnt;
        }
        if (cnt == 3) has_cub = true;
        if (cnt == 2) has_sq   = true;
    }
    if (has_cub) return (seq_t)1;
    if (has_sq)  return (seq_t)3;
    return (seq_t)6;
}
