/*
 * A067068  a(n) = n* - 2^n, where n* (A003418) = least common multiple of the
 *          numbers [1,...,n].
 * https://oeis.org/A067068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = lcm(1,2,…,n) – 2^n (where n* = lcm(1,…,n))
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 22, i64 n <= 42, i128 n <= 88,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */
#include "../utils/seq_pow.h"            /* seq_pow() */

/* cache of all primes ≤ current prime_limit */
static uint64_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;
static idx_t     prime_limit = 0;

/* ensure prime_cache contains all primes ≤ n */
static void
ensure_primes(idx_t n)
{
    if (n <= prime_limit)
        return;

    /* free old cache (build_primes_upto will allocate new one) */
    free(prime_cache);

    build_primes_upto(n, &prime_cache, &prime_cnt);
    prime_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; a(0) is undefined */
        return 0;

    ensure_primes(n);

    seq_t lcm = 1;
    for (size_t i = 0; i < prime_cnt && prime_cache[i] <= n; ++i) {
        uint64_t p = prime_cache[i];
        /* largest exponent k such that p^k ≤ n */
        uint64_t t = p;
        idx_t    k = 1;
        while (t <= n / p) {      /* n is idx_t (uint64_t) */
            t *= p;
            ++k;
        }
        lcm *= seq_pow((seq_t)p, k);
    }

    seq_t two_pow = seq_pow((seq_t)2, n);
    return lcm - two_pow;
}
