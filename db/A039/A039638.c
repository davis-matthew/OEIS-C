/*
 * A039638  Fixed point of "k -> k/2 or (k-1)/2 until result is prime",
 *          starting with prime(n)-1.
 * https://oeis.org/A039638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 7600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of k → k/2 if k is even, (k-1)/2 if k is odd, starting
   from prime(n)-1 until a prime (or 1) is reached.  This implements
   OEIS sequence A039638.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"
#include "../utils/is_prime_big.h"

/* Cached list of the first primes.  This is updated lazily when more
   primes are needed.  */
static idx_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t prime_limit  = 0;

/* Ensure that at least n primes are available in prime_cache.  */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Compute a reasonable upper bound for the nth prime and build the
       cache up to that bound (with a small safety margin).  */
    idx_t lim = _prime_limit_for_idx(n) + 100;

    if (prime_cache != NULL && prime_limit >= lim)
        return;

    free(prime_cache);
    build_primes(lim, &prime_cache, &prime_cnt);
    prime_limit = lim;
}

/* *********************************************************************
   Main sequence function.
   ********************************************************************* */
seq_t
a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    ensure_primes(n);

    /* Starting value: prime(n) – 1 */
    seq_t k = (seq_t)prime_cache[n - 1] - (seq_t)1;

    /* Handle the trivial case directly */
    if (k <= (seq_t)1)
        return k;

    /* Iterate until a prime (or 1) is obtained.  */
    while (!is_prime_big(k)) {
        if (k <= (seq_t)1)
            return (seq_t)1;
        if (k & (seq_t)1)           /* odd */
            k = (k - (seq_t)1) >> 1;
        else                        /* even */
            k >>= 1;
    }
    return k;
}
