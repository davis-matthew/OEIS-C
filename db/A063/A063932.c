/*
 * A063932  Average of largest prime less than or equal to n and smallest
 *          prime greater than or equal to n.
 * https://oeis.org/A063932
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A007917(n) + A007918(n)) / 2
 * Range:     u8 n <= 137, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a small on‑demand sieve of primes.  The sieve limit is
   increased as necessary to include the next prime after n. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

static idx_t *primes = NULL;      /* array of primes in ascending order */
static size_t primes_cnt = 0;     /* number of primes stored */
static idx_t primes_limit = 0;    /* maximum prime value generated */

/* ensure that primes are generated up to at least 'limit' (inclusive) */
static void ensure_primes_up_to(idx_t limit)
{
    if (primes_limit >= limit)
        return;
    if (primes)
        free(primes);
    sieve_primes(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* binary search for the largest prime ≤ n */
static idx_t prev_prime_in_array(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)primes_cnt - 1;
    idx_t res = 0;                    /* 0 means not found yet */
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) / 2;
        if (primes[mid] <= n)
        {
            res = primes[mid];
            lo = mid + 1;
        }
        else
        {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return res;
}

/* binary search for the smallest prime ≥ n */
static idx_t next_prime_in_array(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)primes_cnt - 1;
    idx_t res = 0;                    /* 0 means not found yet */
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) / 2;
        if (primes[mid] >= n)
        {
            res = primes[mid];
            if (mid == 0)
                break;
            hi = mid - 1;
        }
        else
        {
            lo = mid + 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* undefined below offset 2 */

    /* keep sieve large enough to include the next prime after n */
    const idx_t buffer = 1000;
    ensure_primes_up_to(n + buffer);

    idx_t low  = prev_prime_in_array(n);
    idx_t high = next_prime_in_array(n);

    /* The sequence definition is (low + high) / 2.
       Compute in seq_t to avoid overflow. */
    seq_t sum = (seq_t)low + (seq_t)high;
    return sum / 2;
}
