/*
 * A164313  LCM of all differences of odd primes up to prime(n).
 * https://oeis.org/A164313
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ prime(i)-prime(j) : 1<j<i≤n }. It is equal to the
 *            LCM of all even numbers ≤ prime(n)-3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Utility headers (provided by the build) */
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_seq.h"

/* Cached array of primes */
static uint64_t *prime_cache = NULL;
static size_t   prime_cache_cnt = 0;
static idx_t    prime_cache_lim  = 0;

/* Ensure we have all primes up to at least `lim` in the cache. */
static void
ensure_prime_cache(idx_t lim)
{
    if (prime_cache_lim >= lim)
        return;
    if (prime_cache)
        free(prime_cache);
    /* build_primes_upto outputs an array of 64‑bit primes */
    build_primes_upto(lim, &prime_cache, &prime_cache_cnt);
    prime_cache_lim = lim;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3.  Outside that range it is
       unspecified – we return 0. */
    if (n < 3)
        return 0;

    /* Retrieve prime(n).  The primes are numbered starting at 1. */
    uint64_t pn = get_prime(n);
    seq_t    L  = (seq_t)pn - (seq_t)3;   /* largest even difference */

    /* Make sure the prime cache covers all primes ≤ L.  Since L is an
       even number ≤ pn-3, casting it to idx_t is safe for the test limits. */
    ensure_prime_cache((idx_t)L);

    /* Start with the prime factor 2.  Find the largest e with 2^e ≤ L. */
    idx_t   e2  = 0;
    seq_t   tmp = L;
    while (tmp >= 2) {
        tmp /= 2;
        ++e2;
    }
    seq_t result = pow_seq((seq_t)2, (int)e2);

    /* For odd primes, we need the largest e with q^e ≤ L/2.
       L/2 is the largest odd difference (prime_i - 3)/2. */
    seq_t half = L / (seq_t)2;
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t q64 = prime_cache[i];
        if (q64 == 2)        /* already handled */
            continue;
        if (q64 > (uint64_t)L)
            break;
        seq_t q = (seq_t)q64;
        /* Compute exp = floor(log_q(half)) */
        int exp = 0;
        seq_t r = half;
        while (r >= q) {
            r /= q;
            ++exp;
        }
        if (exp > 0)
            result = seq_mul(result, pow_seq(q, exp));
    }

    return result;
}
