/*
 * A075986  Numerator of 1+1/prime(1)^2+ ... + 1/prime(n)^2 where prime(k) is
 *          the k-th prime.
 * https://oeis.org/A075986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 * Recurrence: a(n) = a(n-1)*prime(n)^2 + (∏_{k=1}^{n-1} prime(k))^2,
 *  where prime(k) is the k‑th prime (prime(1)=2).
 *
 * This implementation keeps a lazily‑filled cache of the sequence
 * values and the list of primes.  It grows the arrays on demand and
 * works for any unsigned integer type (8/16/32/64/128 bits) or an
 * arbitrary‑precision class provided by the build.  All arithmetic is
 * performed only with the operators available for seq_t, so the code
 * is portable across the testing environments.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------
 *  Local static state
 * ------------------------------------------------------------------- */
static seq_t *a_cache  = NULL;   /* cache of computed a(n) values   */
static size_t a_len    = 0;      /* number of values stored in cache*/
static size_t a_cap    = 0;      /* allocated capacity of a_cache   */

static idx_t *primes   = NULL;   /* list of primes (prime(1)=2)     */
static size_t primes_len = 0;     /* number of primes stored         */
static size_t primes_cap = 0;     /* allocated capacity of primes    */

static seq_t prod   = 1; /* Π_{k=1}^{i-1} prime(k) */
static seq_t a_prev = 1; /* a(i-1)                */

/* -------------------------------------------------------------------
 *  Helper: grow the a_cache array
 * ------------------------------------------------------------------- */
static void ensure_a_cache_cap(size_t needed)
{
    if (a_cap >= needed)
        return;
    a_cap = a_cap ? a_cap * 2 : 16;
    while (a_cap < needed)
        a_cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
}

/* -------------------------------------------------------------------
 *  Helper: grow the prime array
 * ------------------------------------------------------------------- */
static void ensure_prime_cap(size_t needed)
{
    if (primes_cap >= needed)
        return;
    primes_cap = primes_cap ? primes_cap * 2 : 16;
    while (primes_cap < needed)
        primes_cap *= 2;
    primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
}

/* -------------------------------------------------------------------
 *  Trial‑division primality test using the current prime list
 * ------------------------------------------------------------------- */
static bool is_prime_candidate(idx_t n)
{
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------
 *  Ensure that at least k primes are generated
 * ------------------------------------------------------------------- */
static void ensure_primes(size_t k)
{
    if (primes_len >= k)
        return;

    if (primes == NULL) {
        primes_cap = 16;
        primes     = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        primes_len = 0;
        primes[0]  = 2;          /* prime(1) */
        primes_len = 1;
    }

    idx_t candidate = primes_len == 0 ? 2 : primes[primes_len - 1] + 1;
    while (primes_len < k) {
        while (!is_prime_candidate(candidate))
            ++candidate;
        if (primes_len == primes_cap)
            ensure_prime_cap(primes_len + 1);
        primes[primes_len++] = candidate;
        candidate += 1;
        if (candidate == 2)  /* wrap around if we hit the even number 2 */
            candidate = 3;
    }
}

/* -------------------------------------------------------------------
 *  Grow the a(n) cache up to index n (inclusive)
 * ------------------------------------------------------------------- */
static void ensure_a_cache(size_t n)
{
    if (a_len > n)
        return;                 /* already computed */

    /* Allocate cache on first use */
    if (a_cache == NULL) {
        a_cap = 16;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        a_len    = 1;
        a_prev   = (seq_t)1;
        prod     = (seq_t)1;
    }

    /* Ensure we have at least a_len primes (prime(1)...prime(a_len)) */
    ensure_primes(a_len);

    for (size_t idx = a_len; idx <= n; ++idx) {
        /* prime(idx) is primes[idx-1] because prime(1)=primes[0] */
        idx_t   p   = primes[idx - 1];
        seq_t   p_s = (seq_t)p;
        seq_t   p_sq = p_s * p_s;

        seq_t   prod_sq = prod * prod;        /* (Π_{k=1}^{idx-1} prime(k))^2 */
        seq_t   a_new   = a_prev * p_sq + prod_sq;

        ensure_a_cache_cap(idx + 1);
        a_cache[idx] = a_new;

        /* Prepare for next iteration */
        prod   = prod * p_s;   /* Π_{k=1}^{idx} prime(k)            */
        a_prev = a_new;        /* a(idx) becomes a(idx-1) for next */

        /* After use of p, next prime will be available by loop */
    }

    a_len = n + 1;
}

/* -------------------------------------------------------------------
 *  Public function required by the test harness
 * ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    ensure_a_cache((size_t)n);
    return a_cache[(size_t)n];
}
