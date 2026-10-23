/*
 * A130767  a(n) = product{k=1 to n} (k-th integer from among those positive
 *          integers which are coprime to (n+1-k)).
 * https://oeis.org/A130767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (k-th positive integer coprime to (n+1-k))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   _coprime_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/_coprime_count.h"

/* ------------------------------------------------------------ */
/*  Sieve data (built up to the largest N needed for any call) */
/* ------------------------------------------------------------ */
static idx_t _sieve_limit = 0;      /* largest N for which data was built   */
static uint32_t *_spf = NULL;       /* smallest prime factor for each N      */
static uint32_t *_primes = NULL;    /* list of primes up to _sieve_limit     */
static size_t _prime_count = 0;     /* number of primes found                */

/* ------------------------------------------------------------ */
/*  Build a linear sieve up to 'max_n'                        */
/* ------------------------------------------------------------ */
static void _init_sieve(idx_t max_n)
{
    if (max_n <= _sieve_limit) return;              /* already built */

    /* free previous data, if any */
    free(_spf);
    free(_primes);

    /* allocate arrays */
    _spf = (uint32_t*)calloc((size_t)(max_n + 1), sizeof(uint32_t));
    _primes = (uint32_t*)malloc((size_t)(max_n + 1) * sizeof(uint32_t));

    if (_spf == NULL || _primes == NULL) {
        /* In a real program we would abort or handle the error, but for
           the OEIS harness aborting is acceptable. */
        abort();
    }

    /* linear sieve: compute smallest prime factor and primes list */
    size_t pi = 0;
    for (idx_t i = 2; i <= max_n; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = (uint32_t)i;
            _primes[pi++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pi && _primes[j] <= _spf[i] &&
                          (idx_t)i * _primes[j] <= max_n; ++j) {
            _spf[i * _primes[j]] = _primes[j];
        }
    }
    _prime_count = pi;
    _sieve_limit = max_n;
}

/* ------------------------------------------------------------ */
/*  Count how many integers ≤ x are coprime to a number with   */
/*  prime factors stored in 'pf[0..d-1]'.                      */

/* ------------------------------------------------------------ */
/*  Find the k-th positive integer coprime to N.                */
/* ------------------------------------------------------------ */
static uint64_t _kth_coprime(uint32_t N, uint64_t k)
{
    /* gather distinct prime factors of N */
    uint32_t pf[16];     /* sufficient for N <= 10000 */
    size_t d = 0;
    uint32_t temp = N;
    uint32_t last = 0;
    while (temp > 1) {
        uint32_t p = _spf[temp];
        if (p != last) {
            pf[d++] = p;
            last = p;
        }
        while (temp % p == 0) temp /= p;
    }

    /* binary search for the smallest number whose coprime count ≥ k */
    uint64_t low  = 1;
    uint64_t high = k * (uint64_t)N;          /* safe upper bound */
    if (high < k) high = k;                  /* guard against overflow */

    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (_coprime_count(mid, pf, d) >= k)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ------------------------------------------------------------ */
/*  Public function required by the test harness               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is never requested */

    /* ensure sieve data large enough for the largest N we will use */
    _init_sieve(n);

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t m = (uint32_t)(n + 1 - k);
        uint64_t cop = _kth_coprime(m, (uint64_t)k);
        result *= (seq_t)cop;
    }
    return result;
}
