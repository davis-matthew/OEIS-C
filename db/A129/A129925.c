/*
 * A129925  Triangular sequence based on A002301 and the alternating groups a
 *          prime -adic: t(n,m)=n!/Prime[m] for n>=Prime[m].
 * https://oeis.org/A129925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = n! / p_k where p_k is the k‑th prime ≤ n (k = 1 … π(n)),
 *            the sequence is flattened by n starting with n = 2. The first
 *            term a(1) = 1. The implementation uses a sieve up to a fixed
 *            limit sufficient for all requested indices (up to at least 10
 *            000)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   _a129925_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_a129925_factorial.h"

static bool          _a129925_inited   = false;
static idx_t         _a129925_limit   = 0;
static uint32_t     *_a129925_primes  = NULL;   /* ascending primes   */
static size_t        _a129925_prime_count = 0;
static idx_t        *_a129925_pi      = NULL;   /* pi[i] = #primes ≤ i */
static idx_t        *_a129925_cum     = NULL;   /* cum[i] = Σ_{k=2}^i pi[k] */

/* ----------------------------------------------------------------------- */
/*  Build sieve and precompute pi[] and cum[]                               */
static void _a129925_init(void)
{
    if (_a129925_inited) return;

    const idx_t LIMIT = 200000;           /* enough for n ≤ 10 000          */
    _a129925_limit = LIMIT;

    uint8_t *is_prime = (uint8_t*)malloc((size_t)(LIMIT + 1));
    if (!is_prime) abort();

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    _a129925_primes = (uint32_t*)malloc((size_t)(LIMIT / 2 + 10) * sizeof(uint32_t));
    if (!_a129925_primes) abort();

    _a129925_pi   = (idx_t*)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));
    _a129925_cum  = (idx_t*)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));
    if (!_a129925_pi || !_a129925_cum) abort();

    _a129925_pi[0] = 0;
    _a129925_pi[1] = 0;
    _a129925_cum[0] = 0;
    _a129925_cum[1] = 0;

    idx_t count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            _a129925_primes[_a129925_prime_count++] = (uint32_t)i;
            ++count;
        }
        _a129925_pi[i]   = count;
        _a129925_cum[i]  = _a129925_cum[i - 1] + _a129925_pi[i];
    }

    _a129925_inited = true;
    free(is_prime);
}

/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/*  Public sequence function                                                */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* offset value           */

    if (!_a129925_inited) _a129925_init();

    /* Binary search for the smallest row m ≥ 2 with cum[m] ≥ n          */
    idx_t lo = 2, hi = _a129925_limit;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (_a129925_cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    idx_t prev_cum = (_a129925_cum[row - 1]);   /* cum up to previous row */
    idx_t offset   = n - prev_cum;              /* 1‑based in this row   */

    /* Guard against unexpected out‑of‑range request */
    if (offset == 0 || offset > _a129925_pi[row]) return 0;

    uint32_t prime = _a129925_primes[offset - 1];   /* offset‑th prime overall */
    seq_t fact = _a129925_factorial(row);
    return fact / (seq_t)prime;
}
