/*
 * A164962  a(n) is the least prime from the union {2,3} and A164333,
 *          beginning with which the n-th prime p_n is obtained by some number
 *          of iterations of the S operator g(see A164960).
 * https://oeis.org/A164962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, uned
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   _lower_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164962
 *  a(n) = least prime from the set {2,3} ∪ A164333 such that by repeated
 *         application of g(p)=least prime > 2p one obtains the nth prime p_n.
 *  The algorithm builds the directed graph defined by g and propagates the
 *  minimal seed for each prime by increasing order, so a(n) = minSeed[p_n].
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_lower_bound.h"

#define LIMIT 200000          /* sufficient for the first 10 000 primes */
#define MAX_PRIMES 20000

/* forward declarations */
static size_t _lower_bound(const uint32_t *arr, size_t n, uint32_t val);
static void _build_min_seed(void);
static void _init_primes(void);

/* static data for primes and minimal seeds */
static bool _initialized = false;
static uint8_t _is_prime[LIMIT + 1];
static uint32_t _primes[MAX_PRIMES];
static size_t _prime_cnt = 0;
static seq_t * _min_seed = NULL;

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*  Prime sieve and prime list initialization                               */
static void _init_primes(void)
{
    if (_initialized) return;
    _initialized = true;

    memset(_is_prime, 1, sizeof _is_prime);
    _is_prime[0] = _is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (_is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                _is_prime[j] = 0;

    _prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (_is_prime[i] && _prime_cnt < MAX_PRIMES)
            _primes[_prime_cnt++] = (uint32_t)i;
}

/* ------------------------------------------------------------------------- */
/*  Build array of minimal seed for every prime up to LIMIT                  */
static void _build_min_seed(void)
{
    _init_primes();

    _min_seed = (seq_t *)malloc(_prime_cnt * sizeof(seq_t));
    if (!_min_seed) exit(1); /* fatal error if allocation fails */

    /* Seeds: 2 and 3 are always seeds */
    if (_prime_cnt >= 1) _min_seed[0] = (seq_t)_primes[0]; /* 2 */
    if (_prime_cnt >= 2) _min_seed[1] = (seq_t)_primes[1]; /* 3 */

    for (size_t i = 2; i < _prime_cnt; ++i) {
        uint32_t p = _primes[i];
        uint32_t p_prev = _primes[i - 1];

        uint32_t lower_q = p_prev / 2 + 1;          /* q > p_prev/2       */
        uint32_t upper_q = (p - 1) / 2;             /* q <= (p-1)/2       */

        size_t idx = _lower_bound(_primes, _prime_cnt, lower_q);
        seq_t best = (seq_t)-1;
        bool have = false;

        /* iterate over primes q in [lower_q, upper_q] */
        while (idx < _prime_cnt && _primes[idx] <= upper_q) {
            /* the predecessor satisfies next_prime(2q)=p by construction */
            seq_t seed = _min_seed[idx];
            if (!have || seed < best) {
                best = seed;
                have = true;
            }
            ++idx;
        }

        _min_seed[i] = have ? best : (seq_t)p;  /* seed if no predecessor */
    }
}

/* ------------------------------------------------------------------------- */
/*  Public sequence function                                                 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* out of range */

    if (!_min_seed) _build_min_seed();         /* lazy initialization */

    /* n is 1‑based; primes array is 0‑based */
    if ((size_t)n > _prime_cnt) return (seq_t)0;

    return _min_seed[n - 1];
}
