/*
 * A072057  Sum of divisors of 2*prime(n)+1.
 * https://oeis.org/A072057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors of 2*prime(n)+1
 * Range:     u8 n <= 24, u16 n <= 1871, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A072057.               */

#include <stdlib.h>
#include <stdint.h>

/* Utility functions provided by the build environment. */
#include "../utils/_prime_limit_for_idx.h"   /* idx_t _prime_limit_for_idx(idx_t n); */
#include "../utils/build_primes_upto.h"      /* void build_primes_upto(idx_t limit,
                                                 uint64_t **primes_out,
                                                 size_t *cnt_out); */

static uint64_t * _primes = NULL;      /* array of primes up to the current limit */
static size_t    _primes_cnt = 0;      /* number of primes in _primes           */
static idx_t     _limit_used = 0;      /* highest limit for which _primes was built */

/* Rebuilds the prime array if the requested nth prime exceeds
   the current cached number of primes. */
static void _ensure_primes(idx_t n)
{
    if (n <= _primes_cnt)
        return;                /* we already have at least n primes */

    /* Determine an upper bound on the nth prime and ensure the sieve
       covers it.  The build provides a helper function that returns a
       suitable bound. */
    idx_t target_limit = _prime_limit_for_idx(n);
    if (target_limit <= _limit_used)
        return;                /* existing sieve is already large enough */

    /* Free the existing array (if any) and allocate a new one. */
    if (_primes)
        free(_primes);

    /* build_primes_upto allocates an array of primes <= target_limit. */
    build_primes_upto(target_limit, &_primes, &_primes_cnt);
    _limit_used = target_limit;

    /* If, for some reason, the array does not contain enough primes
       (this should not happen for reasonable limits), bump the limit
       and rebuild.  The loop is unlikely to execute more than once. */
    while (_primes_cnt < (size_t)n) {
        target_limit *= 2;
        if (_primes)
            free(_primes);
        build_primes_upto(target_limit, &_primes, &_primes_cnt);
        _limit_used = target_limit;
    }
}

/* Computes σ(m), the sum of positive divisors of m, assuming that
   a prime factorization table is available in _primes.  The argument
   m fits in uint64_t.  The return value is stored in seq_t. */
static seq_t _sigma(uint64_t m)
{
    seq_t result = 1;
    uint64_t rem = m;

    for (size_t i = 0; i < _primes_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p * p > rem)
            break;

        if (rem % p != 0)
            continue;

        seq_t term = 1;         /* term for this prime factor */
        seq_t p_pow = 1;        /* current power of p */

        while (rem % p == 0) {
            rem /= p;
            p_pow = (seq_t)p_pow * (seq_t)p;  /* multiply by p */
            term += p_pow;                     /* add new power */
        }

        result *= term;                      /* combine with previous primes */
    }

    /* If a prime factor larger than the sieve limit remains, it must
       occur only once. */
    if (rem > 1)
        result *= ((seq_t)1 + (seq_t)rem);

    return result;
}

/* The OEIS sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return 0;

    _ensure_primes(n);                     /* ensure we have the first n primes */

    uint64_t p = _primes[n - 1];            /* prime(n)  (0‑based index) */
    uint64_t m = 2ULL * p + 1ULL;           /* 2*prime(n)+1 */

    return _sigma(m);
}
