/*
 * A065682  Number of primes <= prime(n) which begin with a 3.
 * https://oeis.org/A065682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   starts_with_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of primes ≤ prime(n) whose decimal representation begins with the digit 3.
   The code generates primes on demand using a simple sieve; it keeps a cache of the
   primes up to the largest n requested so far.  Only integer arithmetic is used,
   and all allocations are cast (C++ compatible).  This implementation works for
   the C11 and C++17 builds used by the OEIS test harness. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/starts_with_3.h"

static uint32_t *cached_primes = NULL;       /* array of primes up to cached_limit      */
static size_t  cached_primes_cnt = 0;        /* number of primes stored                 */
static size_t  cached_limit = 0;             /* upper bound that was sieved             */
static idx_t   cached_n = 0;                 /* largest n for which primes were cached */

/* --- helpers --------------------------------------------------------- */

/* Estimate an upper bound for the n‑th prime (Rosser–Schoenfeld). */
static size_t estimate_prime_limit(idx_t n)
{
    if (n < 6) return 15;                    /* 2,3,5,7,11,13 */
    double nd = (double)n;
    double logn  = log(nd);
    double loglogn = log(logn);
    return (size_t)(nd * (logn + loglogn) + 3.0);
}

/* Sieve of Eratosthenes up to 'limit', store primes in cached_primes. */
static void ensure_cache(idx_t n)
{
    if (n <= cached_n) return;              /* cache already sufficient */

    size_t limit = estimate_prime_limit(n);

    char *sieve = (char *)malloc(limit + 1);
    if (!sieve) return;                     /* allocation failure – unlikely */

    for (size_t i = 0; i <= limit; ++i) sieve[i] = 0;

    size_t sqrt_limit = (size_t)floor(sqrt((double)limit));
    for (size_t i = 2; i <= sqrt_limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    size_t cap = limit / 10 + 10;            /* heuristic capacity */
    uint32_t *primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) { free(sieve); return; }
    size_t cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (cnt == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) {
                    free(primes);
                    free(sieve);
                    return;
                }
                primes = tmp;
            }
            primes[cnt++] = (uint32_t)i;
        }
    }

    free(sieve);
    free(cached_primes);
    cached_primes      = primes;
    cached_primes_cnt  = cnt;
    cached_limit       = limit;
    cached_n           = n;
}

/* --- main function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset is 1, but safety for 0 */

    ensure_cache(n);

    seq_t count = 0;
    for (idx_t i = 0; i < n && i < (idx_t)cached_primes_cnt; ++i)
        if (starts_with_3(cached_primes[i])) ++count;

    return count;
}
