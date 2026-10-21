/*
 * A085272  a(n) = lcm_{k=1..n} (prime(k) + 1).
 * https://oeis.org/A085272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm_{k=1..n} (prime(k)+1) (index offset 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  File‑scope cache for primes and the corresponding lcm values.        */
/* --------------------------------------------------------------------- */
static idx_t   *prime_cache   = NULL;   /* prime_cache[i] = prime(i+1)   */
static seq_t   *lcm_cache     = NULL;   /* lcm_cache[i]   = a(i+1)      */
static idx_t   prime_count   = 0;       /* number of primes stored      */
static idx_t   lcm_count     = 0;       /* number of lcm values stored  */
static size_t  cache_cap     = 0;       /* allocated capacity for arrays*/


/* --------------------------------------------------------------------- */
/*  Euclidean GCD for seq_t.                                               */
/* --------------------------------------------------------------------- */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------------------------------------------------------------------- */
/*  Trial division prime test using primes already stored.                 */
/* --------------------------------------------------------------------- */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2)
        return false;
    for (idx_t i = 0; i < prime_count && prime_cache[i] * prime_cache[i] <= n; ++i)
    {
        if (n % prime_cache[i] == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Grow the cache until it contains at least n primes (and thus n lcm values).  */
/* --------------------------------------------------------------------- */
static void ensure_capacity(idx_t n)
{
    /* Initialise on first call */
    if (cache_cap == 0)
    {
        cache_cap = 64;
        prime_cache = (idx_t*)malloc(cache_cap * sizeof(idx_t));
        lcm_cache   = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!prime_cache || !lcm_cache)
            abort();
    }

    /* Generate primes and lcms until we have at least n entries */
    while (prime_count <= n)
    {
        /* Find next candidate prime */
        idx_t candidate = (prime_count == 0) ? 2 : prime_cache[prime_count - 1] + 1;
        if (candidate <= 2)
            candidate = 2;
        else if (candidate % 2 == 0)
            ++candidate;           /* skip even numbers after 2 */

        while (!is_prime_candidate(candidate))
            candidate += 2;        /* test only odd numbers */

        /* Grow arrays if necessary */
        if (prime_count >= cache_cap)
        {
            cache_cap <<= 1;
            prime_cache = (idx_t*)realloc(prime_cache, cache_cap * sizeof(idx_t));
            lcm_cache   = (seq_t*)realloc(lcm_cache, cache_cap * sizeof(seq_t));
            if (!prime_cache || !lcm_cache)
                abort();
        }

        /* Store prime */
        prime_cache[prime_count] = candidate;

        /* Compute corresponding lcm value */
        seq_t prime_plus_one = (seq_t)candidate + 1;
        if (prime_count == 0)
        {
            lcm_cache[prime_count] = prime_plus_one;  /* a(1) */
        }
        else
        {
            seq_t prev_lcm = lcm_cache[prime_count - 1];
            seq_t g        = seq_gcd(prev_lcm, prime_plus_one);
            seq_t tmp      = prev_lcm / g;
            seq_t new_lcm  = tmp * prime_plus_one;
            lcm_cache[prime_count] = new_lcm;
        }

        ++prime_count;
        ++lcm_count;
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the test harness.                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                       /* behaviour for non‑positive indices is irrelevant */

    ensure_capacity(n);                 /* make sure the cache holds n entries */
    return lcm_cache[n - 1];            /* lcm_cache is 0‑indexed, a(1) is lcm_cache[0] */
}
