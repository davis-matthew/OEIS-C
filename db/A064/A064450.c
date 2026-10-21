/*
 * A064450  Sum of totients of binomial coefficients C(n,j), j=0..n.
 * https://oeis.org/A064450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=0..n} phi(C(n,j)) (C(n,j) = binomial coefficient)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>        /* for memset */
#include "../utils/pow_int.h"   /* pow_int(seq_t base, idx_t exp) */

/* static cache for primes and factorisation helpers */
static idx_t     _max_n      = 0;               /* largest n handled     */
static uint32_t* _primes     = NULL;            /* array of primes       */
static size_t    _prime_cnt  = 0;               /* number of primes      */
static int*      _prime_idx  = NULL;            /* p -> index in _primes */
static uint32_t* _spf        = NULL;            /* smallest prime factor */

/* exp array (per call) */
static int*      _exp        = NULL;            /* exponents of primes   */
static size_t    _exp_cap    = 0;               /* current capacity      */

/* --------------------------------------------------------------------
   Build prime list and smallest prime factor array up to 'limit'.
   Uses a linear sieve.  The arrays are rebuilt only when a larger
   limit is requested.  All indices and sizes are indexed by idx_t.
   -------------------------------------------------------------------- */
static void build_sieve(idx_t limit)
{
    if (limit <= _max_n) return;                 /* nothing to do */

    /* free old data if any */
    if (_primes)   free(_primes);
    if (_prime_idx) free(_prime_idx);
    if (_spf)     free(_spf);

    _primes   = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    _prime_idx = (int*)     malloc((size_t)(limit + 1) * sizeof(int));
    _spf      = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));

    if (!_primes || !_prime_idx || !_spf) {
        /* memory error – in this context we abort */
        abort();
    }

    /* initialisation */
    for (size_t i = 0; i <= (size_t)limit; ++i) {
        _spf[i] = 0;
        _prime_idx[i] = -1;
    }

    size_t pc = 0;                               /* prime counter */

    for (idx_t i = 2; i <= limit; ++i) {
        if (_spf[i] == 0) {                      /* i is prime */
            _spf[i] = (uint32_t)i;
            _primes[pc] = (uint32_t)i;
            _prime_idx[i] = (int)pc;
            ++pc;
        }

        for (size_t k = 0; k < pc; ++k) {
            uint32_t p = _primes[k];
            idx_t prod = i * (idx_t)p;
            if (prod > limit) break;
            if (_spf[prod] == 0) {
                _spf[prod] = p;
            }
        }
    }

    _prime_cnt = pc;
    _max_n = limit;
}

/* --------------------------------------------------------------------
   Compute a(n) = sum_{j=0..n} phi(C(n,j))
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)1;

    /* prepare sieve data for this n */
    build_sieve(n);

    /* allocate / reallocate exponent array */
    if (_exp == NULL || _exp_cap < _prime_cnt) {
        free(_exp);
        _exp = (int*)malloc(_prime_cnt * sizeof(int));
        if (!_exp) abort();
        _exp_cap = _prime_cnt;
    }
    memset(_exp, 0, _prime_cnt * sizeof(int));

    seq_t total = (seq_t)1;                      /* j = 0, C(n,0)=1, phi=1 */

    for (idx_t j = 1; j <= n; ++j) {
        /* numerator (n - j + 1) : add its prime factors */
        uint32_t num = (uint32_t)(n - j + 1);
        uint32_t x = num;
        while (x > 1) {
            uint32_t p = _spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            size_t pi = (size_t)_prime_idx[p];
            _exp[pi] += cnt;
        }

        /* denominator j : subtract its prime factors */
        x = (uint32_t)j;
        while (x > 1) {
            uint32_t p = _spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            size_t pi = (size_t)_prime_idx[p];
            _exp[pi] -= cnt;
        }

        /* compute phi from current exponents */
        seq_t phi = (seq_t)1;
        for (size_t pi = 0; pi < _prime_cnt; ++pi) {
            int e = _exp[pi];
            if (e > 0) {
                seq_t base = (seq_t)_primes[pi];
                if (e == 1) {
                    phi *= (base - (seq_t)1);
                } else {
                    seq_t pow_val = pow_int(base, (idx_t)(e - 1));
                    phi *= pow_val;
                    phi *= (base - (seq_t)1);
                }
            }
        }

        total += phi;
    }

    return total;
}
