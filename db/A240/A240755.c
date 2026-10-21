/*
 * A240755  Smallest prime which occurs in prime power factorization of
 *          A240751(n)! with exponent n.
 * https://oeis.org/A240755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A240755:  a(n) = the smallest prime that occurs with exponent n
   in the prime‑power factorization of A240751(n)!.

   A240751(n) is the minimal m for which some prime power p^n divides m!.
   The prime p that achieves this exponent in that minimal factorial is
   returned by this function.  The implementation searches primes up to a
   bound that is always larger than the required minimal factorial.
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* gives build_primes_upto() */

static inline idx_t exp_in_fact(idx_t m, uint64_t p)
{
    idx_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* Return the smallest m such that v_p(m!) >= n, or 0 if impossible.
   The function uses a binary search on the monotone function v_p(m!). */
static inline idx_t min_m_with_exponent(uint64_t p, idx_t n)
{
    if (n == 0)
        return 0;

    idx_t low = p;                /* exponent is 0 for m < p */
    idx_t high = n * p;           /* safe upper bound */
    if (high < low)
        high = low;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (exp_in_fact(mid, p) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;                    /* first m with exp >= n */
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* not part of the official offset */

    /* static storage for the prime table */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t prime_limit = 0;

    /* first bound : generous enough for all practical n (≤10000) */
    idx_t limit = (idx_t)(20 * n + 200);
    if (limit < 200)
        limit = 200;

    /* ensure primes up to 'limit' are available */
    if (limit > prime_limit) {
        if (primes) {
            free(primes);
            primes = NULL;
            primes_cnt = 0;
        }
        build_primes_upto(limit, &primes, &primes_cnt);
        prime_limit = limit;
    }

    idx_t best_m = (idx_t)(-1);          /* minimal m found so far */
    seq_t  best_p = 0;                   /* corresponding prime */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];

        /* no chance to improve after p > best_m once a candidate exists */
        if (best_m != (idx_t)(-1) && p > best_m)
            break;

        /* find minimal m where exponent of p in m! is at least n */
        idx_t m = min_m_with_exponent(p, n);
        if (m == 0 || exp_in_fact(m, p) != n)
            continue;                  /* this prime never attains n */

        /* update best candidate */
        if (best_m == (idx_t)(-1) || m < best_m ||
            (m == best_m && p < (uint64_t)best_p)) {
            best_m = m;
            best_p = (seq_t)p;
        }
    }

    /* If we did not find a prime within the current limit, enlarge the
       table and try again.  This loop will terminate quickly because
       the required m is always O(n). */
    while (best_m == (idx_t)(-1)) {
        limit *= 2;
        free(primes);
        primes = NULL;
        primes_cnt = 0;
        build_primes_upto(limit, &primes, &primes_cnt);
        prime_limit = limit;

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];

            idx_t m = min_m_with_exponent(p, n);
            if (m == 0 || exp_in_fact(m, p) != n)
                continue;

            if (best_m == (idx_t)(-1) || m < best_m ||
                (m == best_m && p < (uint64_t)best_p)) {
                best_m = m;
                best_p = (seq_t)p;
            }
        }
    }

    return best_p;
}
