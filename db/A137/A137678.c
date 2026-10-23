/*
 * A137678  Consider the consecutive composite numbers between prime(n) and
 *          prime(n+1). Letting k = prime(n+1)-prime(n)-1, a(n) is the number
 *          of these numbers that have all prime factors less than k.
 * https://oeis.org/A137678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A137678:
   a(n) = number of composite numbers between prime(n) and prime(n+1)
          whose all prime factors are strictly less than
          k = prime(n+1) - prime(n) - 1. */
#include <stdint.h>
#include <stdbool.h>

/* utilities already available */
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

static bool initialized = false;
static uint64_t *primes = NULL;      /* primes[i] = the (i+1)-th prime */
static size_t primes_cnt = 0;
static uint64_t *spf = NULL;        /* spf[n] = smallest prime factor of n */

static void ensure_init(void)
{
    if (!initialized) {
        /* Build primes up to a safe limit covering at least 10001 primes.
           200,000 > the 10,001st prime (104,743). */
        build_primes_upto(200000, &primes, &primes_cnt);
        /* Build smallest prime factor table up to the same limit. */
        build_spf(200000, (idx_t **)&spf);
        initialized = true;
    }
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first pair of primes: 2,3 ... */
    if (n == 0) return 0;           /* safety, though n>=1 per spec */

    ensure_init();

    /* Ensure that the primes array has index n (which is prime(n+1)). */
    if (n >= primes_cnt) {
        /* In practice this shouldn't happen for n <= 10000 with our limit */
        return 0;
    }

    uint64_t p_n   = primes[n - 1];
    uint64_t p_np1 = primes[n];
    idx_t    k_diff = (idx_t)(p_np1 - p_n - 1);   /* number of composites between */

    if (k_diff == 0) return 0;  /* no composites */

    uint64_t k = k_diff;   /* use for comparisons */
    seq_t    count = 0;

    for (uint64_t x = p_n + 1; x < p_np1; ++x) {
        uint64_t m = x;
        bool ok = true;
        while (m > 1) {
            uint64_t s = spf[m];
            if (s >= k) {
                ok = false;
                break;
            }
            while (m % s == 0) m /= s;
        }
        if (ok) ++count;
    }
    return count;
}
