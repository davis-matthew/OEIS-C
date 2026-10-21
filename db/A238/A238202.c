/*
 * A238202  Primes p such that b = 2*p + 1 is semiprime and c = 2*b + 1 is
 *          3-almost prime.
 * https://oeis.org/A238202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A238202: a(n) is the nth prime p such that b = 2p + 1 is a
 * semiprime (product of two primes, possibly equal) and c = 2b + 1 has
 * exactly three prime factors counted with multiplicity.
 *
 * The algorithm maintains a dynamic list of primes (built via
 * build_primes_upto).  It iterates through the primes and tests the
 * semiprime condition using a small trial‑division routine
 * (smallest prime factor).  Each prime satisfying the condition is
 * counted until the nth is reached.  The state is stored statically
 * so that calls are independent of call order.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Smallest prime factor of x.  Uses trial division; fast enough for
 * the 64‑bit range needed here.  Relies only on 64‑bit integer ops. */
static uint64_t spf_uint64(uint64_t x) {
    if (x < 2) return x;
    if ((x & 1ULL) == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= x; d += 2ULL) {
        if (x % d == 0ULL) return d;
    }
    return x;              /* x is prime */
}

seq_t a(idx_t n) {
    /* Static state to allow arbitrary call order. */
    static uint64_t *primes = NULL;    /* primes array  */
    static size_t primes_cnt = 0;       /* current number of primes */
    static idx_t sieve_limit = 1024;    /* current sieve limit  */
    static idx_t prime_idx = 0;         /* index of next prime to process   */
    static size_t found_cnt = 0;        /* number of qualifying primes found */
    static seq_t last_prime = 0;        /* last qualifying prime */

    /* Grow or replenish the prime table until we have found n terms. */
    while (found_cnt < n) {
        /* If we have exhausted the current list, extend the sieve. */
        if (prime_idx >= (idx_t)primes_cnt) {
            idx_t old_cnt = primes_cnt;
            sieve_limit *= 2;
            free(primes);
            build_primes_upto(sieve_limit, &primes, &primes_cnt);
            prime_idx = old_cnt;                /* start from where we left off */
        }

        uint64_t p = primes[prime_idx++];          /* next prime to examine */

        /* b = 2p + 1 must be semiprime. */
        uint64_t b = 2ULL * p + 1ULL;
        uint64_t spb = spf_uint64(b);
        if (spb == b) continue;                   /* b is prime, reject */

        uint64_t rem = b / spb;
        if (rem < 2ULL || spf_uint64(rem) != rem) continue; /* not semiprime */

        /* c = 2b + 1 must have exactly three prime factors (with multiplicity). */
        uint64_t c = 2ULL * b + 1ULL;
        size_t cnt = 0;
        uint64_t tmp = c;
        while (tmp > 1ULL && cnt <= 3) {
            uint64_t spc = spf_uint64(tmp);
            tmp /= spc;
            ++cnt;
        }
        if (cnt == 3) {
            ++found_cnt;
            last_prime = (seq_t)p;
            if (found_cnt == n) return last_prime;
        }
    }
    return last_prime;   /* should never be reached */
}
