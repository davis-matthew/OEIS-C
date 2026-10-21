/*
 * A220281  a(n) is the smallest number, such that for all N >= a(n) there are
 *          at least n primes between 14*N and 15*N.
 * https://oeis.org/A220281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A220281
 *  a(n) = minimal N such that for all k ≥ N, the number of primes p with 14*k < p ≤ 15*k is at least n.
 *  Computed by building a sieve up to a fixed upper bound and evaluating the prefix minima
 *  of the prime-count differences.  This implementation works for any seq_t/idx_t
 *  that support the required arithmetic operations and for all n ≥ 1 with
 *  Nmax = 200000 (sufficient for n ≤ 10000).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Global data (initialised once)                                        */
/* --------------------------------------------------------------------- */
static bool _initialized = false;

static const idx_t _NMAX = 200000ULL;               /* maximum N processed       */
static const idx_t _LIMIT = 15ULL * _NMAX;          /* sieve upper bound         */

static unsigned char * _prime_flag = NULL; /* _prime_flag[i] == 1 ⇔ i is prime */
static uint32_t       * _prime_pi   = NULL; /* prefix counts of primes    */
static uint32_t       * _suffix_min = NULL; /* suffix minima of h(N)      */

/* --------------------------------------------------------------------- */
/* Sieve of Eratosthenes up to _LIMIT                                 */
/* --------------------------------------------------------------------- */
static void _build_sieve(void)
{
    _prime_flag = (unsigned char *)malloc((size_t)(_LIMIT + 1));
    if (!_prime_flag) exit(1);
    for (idx_t i = 0; i <= _LIMIT; ++i) _prime_flag[i] = 1;
    _prime_flag[0] = _prime_flag[1] = 0;

    idx_t sq = (idx_t)(sqrt((double)_LIMIT) + 1.0);
    for (idx_t p = 2; p <= sq; ++p)
        if (_prime_flag[p])
            for (idx_t j = p * p; j <= _LIMIT; j += p)
                _prime_flag[j] = 0;

    _prime_pi = (uint32_t *)malloc((size_t)(_LIMIT + 1) * sizeof(uint32_t));
    if (!_prime_pi) exit(1);
    uint32_t acc = 0;
    for (idx_t i = 0; i <= _LIMIT; ++i) {
        if (_prime_flag[i]) ++acc;
        _prime_pi[i] = acc;
    }
}

/* --------------------------------------------------------------------- */
/* Build suffix minima of h(N) = π(15N) – π(14N)                          */
/* --------------------------------------------------------------------- */
static void _build_suffix_min(void)
{
    _suffix_min = (uint32_t *)malloc((size_t)(_NMAX + 2) * sizeof(uint32_t));
    if (!_suffix_min) exit(1);

    uint32_t cur_min = UINT32_MAX;
    for (idx_t N = _NMAX; N >= 1; --N) {
        uint32_t h = _prime_pi[15ULL * N] - _prime_pi[14ULL * N];
        if (h < cur_min) cur_min = h;
        _suffix_min[N] = cur_min;
    }
    _suffix_min[0] = cur_min; /* unused but harmless */
}

/* --------------------------------------------------------------------- */
/* Ensure global data is ready                                           */
/* --------------------------------------------------------------------- */
static void _ensure_initialized(void)
{
    if (!_initialized) {
        _build_sieve();
        _build_suffix_min();
        _initialized = true;
    }
}

/* --------------------------------------------------------------------- */
/* Main entry point:  a(n)                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* undefined for n=0, but return 0 to avoid UB */
    if (n == 0) return (seq_t)0;

    _ensure_initialized();

    /* binary search for minimal N with suffix_min[N] ≥ n */
    idx_t low  = 1;
    idx_t high = _NMAX;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (_suffix_min[mid] >= (uint32_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
