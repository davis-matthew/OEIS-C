/*
 * A227324  Result of changing both the prime indices and the exponents in the
 *          prime factorization of n: increment odd values, decrement even
 *          values.
 * https://oeis.org/A227324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏p^e∈prime_factors(n) (q(p))^{e'} * where q(p) is the
 *            neighbouring prime of p: * if the index of p in the list of all
 *            primes is odd → next prime, * if even → previous prime; * e' =
 *            e+1 if e is odd, otherwise e-1. * Thus a(n) is the result of
 *            modifying both the prime bases and their * expon...
 * Range:     u8 n <= 9, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_pow_int.h"

/* static state for primes ------------------------------------------- */
static bool        _primes_initialized = false;
static uint32_t   *_primes          = NULL;   /* list of primes          */
static size_t       _prime_count      = 0;
static uint32_t   *_prime_index    = NULL;   /* 1‑based index of prime   */

/* build all primes up to LIMIT and a lookup table for prime→index      */
static void _init_primes(void)
{
    if (_primes_initialized) return;

    const int LIMIT = 20000;                  /* enough for n ≤ 100 000     */
    bool   *is_comp  = (bool   *)calloc(LIMIT + 1, sizeof(bool));
    uint32_t *tmp_primes = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *idx_tbl    = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));

    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            tmp_primes[_prime_count++] = (uint32_t)i;
            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    /* store primes and their 1‑based indices */
    _primes = tmp_primes;
    _prime_index = idx_tbl;
    for (size_t i = 0; i < _prime_count; ++i)
        _prime_index[_primes[i]] = (uint32_t)(i + 1);

    free(is_comp);
    _primes_initialized = true;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    _init_primes();

    /* a(1) = 1 */
    if (n <= 1) return 1;

    /* factorise n (at most 64‑bit)                             */
    uint64_t tmp = (uint64_t)n;
    uint32_t f_primes[20];
    uint32_t f_exps[20];
    size_t fcnt = 0;

    for (size_t i = 0; i < _prime_count &&
         (uint64_t)_primes[i] * _primes[i] <= tmp; ++i) {
        uint32_t p = _primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            f_primes[fcnt] = p;
            f_exps[fcnt]  = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor > √n          */
        f_primes[fcnt] = (uint32_t)tmp;
        f_exps[fcnt]  = 1;
        ++fcnt;
    }

    /* apply the prime/base–exponent transformation             */
    seq_t result = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        uint32_t p = f_primes[i];
        uint32_t e = f_exps[i];

        /* find neighbour prime q(p) -------------------------------- */
        uint32_t idx = _prime_index[p];           /* 1‑based index         */
        uint32_t next_idx = (idx & 1U) ? idx + 1U : idx - 1U;
        uint32_t q = _primes[next_idx - 1U];

        /* modify exponent ------------------------------------------- */
        uint32_t eprime = (e & 1U) ? e + 1U : e - 1U;

        /* accumulate result ---------------------------------------- */
        seq_t pow = _pow_int((seq_t)q, eprime);
        result *= pow;
    }

    return result;
}
