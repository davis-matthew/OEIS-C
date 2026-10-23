/*
 * A048633  Largest squarefree number dividing n-th central binomial
 *          coefficient C(n,[ n/2 ]).
 * https://oeis.org/A048633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct primes dividing C(n, floor(n/2))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 38, u64 n <= 70, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the largest squarefree divisor of the n‑th central binomial coefficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for prime sieve */
static idx_t _sieve_limit = 0;
static uint64_t *_prime_list = NULL;
static size_t _prime_cnt = 0;

/* Ensure that all primes <= n are in _prime_list */
static void _ensure_primes(idx_t n)
{
    if (n <= _sieve_limit)
        return;

    uint8_t *is_comp = (uint8_t *)calloc((size_t)n + 1, 1);
    if (!is_comp)
        return; /* allocation failure – return with empty slice */

    free(_prime_list);
    _prime_cnt = 0;
    _prime_list = NULL;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            ++_prime_cnt;
            _prime_list = (uint64_t *)realloc(_prime_list, _prime_cnt * sizeof(uint64_t));
            _prime_list[_prime_cnt - 1] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    _sieve_limit = n;
}

/* Legendre exponent of prime p in m! */
static idx_t _legendre(idx_t m, uint64_t p)
{
    idx_t sum = 0;
    idx_t power = p;
    while (power <= m) {
        sum += m / power;
        if (power > m / p)
            break;
        power *= p;
    }
    return sum;
}

/* Compute the product of all distinct primes dividing C(n, floor(n/2)) */
static seq_t _squarefree_central_binom(idx_t n)
{
    if (n <= 1)
        return 1;

    _ensure_primes(n);
    idx_t k = n / 2;            /* floor(n/2) */
    idx_t nk = n - k;           /* ceil(n/2) */

    seq_t res = 1;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _prime_list[i];
        idx_t e = _legendre(n, p) - _legendre(k, p) - _legendre(nk, p);
        if (e > 0)
            res *= (seq_t)p;
    }
    return res;
}

/* Public sequence entry point – n is 1‑based */
seq_t a(idx_t n)
{
    return _squarefree_central_binom(n);
}
