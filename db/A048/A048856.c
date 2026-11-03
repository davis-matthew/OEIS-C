/*
 * A048856  Number of digits of prime(n)# + 1.
 * https://oeis.org/A048856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = number of decimal digits of prime(n)# (primorial of the
 *            nth prime) + 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

/* helpers from the utils library */
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* upper bound for the nth prime (n >= 6) */
static inline idx_t nth_prime_upper(idx_t n)
{
    if (n < 6) return 15;
    long double nn = (long double)n;
    long double ub = nn * (logl(nn) + logl(logl(nn)));
    return (idx_t)(ub + 10.0L);          /* safety margin */
}

/* static cache for primes and cumulative log10 sum */
static uint64_t *prime_list   = NULL;   /* array of all primes up to the current limit */
static size_t    prime_cnt    = 0;      /* total number of primes stored */

/* cum_log[i] = Σ_{k=0}^{i-1} log10( prime_list[k] );  cum_log[0] == 0 */
static long double *cum_log  = NULL;    /* length = cache_len + 1 */
static idx_t        cache_len = 0;      /* largest n for which cum_log[n] is valid */

/* rebuild the cache for at least the first n primes */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;          /* already sufficient */

    /* find a sieve limit that yields at least n primes */
    idx_t limit = nth_prime_upper(n);

    uint64_t *primes = NULL;
    size_t    cnt    = 0;
    build_primes_upto(limit, &primes, &cnt);

    /* occasionally the bound was too low for very small n */
    while (cnt < n) {
        limit *= 2;
        free(primes);
        build_primes_upto(limit, &primes, &cnt);
    }

    /* free old cache */
    free(prime_list);
    free(cum_log);

    /* replace with new data */
    prime_list = primes;
    prime_cnt  = cnt;

    cum_log  = (long double *)malloc(sizeof(long double) * (n + 1));
    cum_log[0] = 0.0L;
    for (idx_t i = 0; i < n; ++i) {
        cum_log[i + 1] = cum_log[i] + log10l((long double)prime_list[i]);
    }
    cache_len = n;
}

/* sequence entry */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0; callers will not use it */

    ensure_cache(n);

    /* number of decimal digits of the primorial: floor(log10) + 1 */
    unsigned long long digits = (unsigned long long)(floorl(cum_log[n]) + 1.0L);
    return (seq_t)digits;
}
