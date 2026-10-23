/*
 * A184728  a(n) = largest k such that A001358(n+1) = A001358(n) + (A001358(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A184728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001358(n) - A065516(n) if 2*A065516(n) < A001358(n), 0
 *            otherwise
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed from the sequence of semiprimes S(n). */

#include <stdint.h>
#include <stdlib.h>

static uint64_t _prv_limit = 200000;            /* upper bound for sieving    */
static uint32_t * _spf = NULL;                 /* smallest prime factor     */
static uint8_t  * _fac = NULL;                 /* number of prime factors (with multiplicity) */
static seq_t    * _semiprimes = NULL;          /* array of semiprimes       */
static size_t    _semiprime_cnt = 0;           /* number of semiprimes      */

static void _init(void) {
    size_t limit = _prv_limit;

    /* Allocate arrays for sieve */
    _spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    _fac = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!_spf || !_fac) { /* allocation failed, abort */ abort(); }

    /* Sieve for smallest prime factor */
    for (size_t i = 0; i <= limit; ++i) _spf[i] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (_spf[i] == 0) {            /* i is prime */
            _spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (_spf[j] == 0) _spf[j] = (uint32_t)i;
            }
        }
    }

    /* Precompute factor counts */
    _fac[0] = _fac[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        _fac[i] = _fac[i / _spf[i]] + 1;
    }

    /* Build list of semiprimes (numbers with exactly two prime factors) */
    _semiprimes = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!_semiprimes) abort();
    for (size_t i = 4; i <= limit; ++i) {
        if (_fac[i] == 2) {
            _semiprimes[_semiprime_cnt++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* index 0 is undefined for OEIS offset 1 */

    static int _initialized = 0;
    if (!_initialized) {
        _init();
        _initialized = 1;
    }

    /* Ensure we have at least n+1 semiprimes (for s_n and s_{n+1}) */
    if (n >= (idx_t)_semiprime_cnt) {
        /* Should not happen for requested n within performance bounds */
        return (seq_t)0;
    }

    seq_t s_n     = _semiprimes[n - 1];
    seq_t s_next  = _semiprimes[n];
    seq_t diff     = s_next - s_n;

    /* a(n) = s_n - diff iff 2*diff < s_n, else 0 */
    if (s_n > diff + diff) {
        return s_n - diff;
    } else {
        return (seq_t)0;
    }
}
