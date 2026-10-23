/*
 * A177501  The number of entries in row n of table A174625 which are
 *          multiples of n.
 * https://oeis.org/A177501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k | 1 ≤ k ≤ ⌊n/2⌋, k divides C(n‑k‑1, k‑1) }|
 * Range:     u8 n <= 520, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   _vp_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for all n ≥ 1. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/_vp_factorial.h"

/* Sieve of Eratosthenes to generate all primes ≤ 10000.
   The array is built only once. */
static idx_t _prime_list[1229];
static size_t _prime_count = 0;
static bool _primes_built = false;

static void _build_primes(void)
{
    if (_primes_built)
        return;
    bool sieve[10001] = {false};
    size_t cnt = 0;
    for (int i = 2; i <= 10000; ++i)
    {
        if (!sieve[i])
        {
            _prime_list[cnt++] = (idx_t)i;
            if ((int64_t)i * i <= 10000)
                for (int j = i * i; j <= 10000; j += i)
                    sieve[j] = true;
        }
    }
    _prime_count = cnt;
    _primes_built = true;
}

/* Main function computing the OEIS sequence A177501. */
seq_t a(idx_t n)
{
    if (n == 1)   /* offset 1 */
        return (seq_t)1;

    _build_primes();

    size_t count = 0;
    idx_t half = n >> 1;                  /* floor(n/2) */
    for (idx_t k = 1; k <= half; ++k)
    {
        idx_t N = n - k - 1;
        idx_t K = k - 1;
        idx_t tmp = k;
        bool ok = true;

        /* factor k using the prime list */
        for (size_t i = 0; i < _prime_count &&
                 (_prime_list[i] * _prime_list[i] <= tmp); ++i)
        {
            idx_t p = _prime_list[i];
            if (tmp % p == 0)
            {
                idx_t e = 0;
                while (tmp % p == 0)
                {
                    ++e;
                    tmp /= p;
                }
                idx_t v = _vp_factorial(N, p) -
                          _vp_factorial(K, p) -
                          _vp_factorial(N - K, p);
                if (v < e)
                {
                    ok = false;
                    break;
                }
            }
        }

        /* remaining prime factor > sqrt(k) */
        if (ok && tmp > 1)
        {
            idx_t p = tmp;
            /* exponent e = 1 for this prime */
            idx_t v = _vp_factorial(N, p) -
                      _vp_factorial(K, p) -
                      _vp_factorial(N - K, p);
            if (v < 1)
                ok = false;
        }

        if (ok)
            ++count;
    }

    return (seq_t)count;
}
