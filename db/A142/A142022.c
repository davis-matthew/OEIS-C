/*
 * A142022  Primes congruent to 18 mod 31.
 * https://oeis.org/A142022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 213, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation for OEIS A142022:
   primes congruent to 18 (mod 31).  The sequence is generated
   on demand by keeping a cache of the found primes and extending
   it as needed.  Primality is tested with a small trial‑division
   routine that uses a static list of primes up to 5000, which
   is sufficient for the 10 000th term (~2.8 × 10⁶). */

/*-------------  Includes and type definitions  -------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*-------------  Static cache of found primes  -------------*/
static seq_t *prime_cache = NULL;   /* array of primes   */
static size_t prime_cnt  = 0;       /* number stored    */
static size_t prime_cap  = 0;       /* array capacity   */

/* Grow the cache to at least 'need' elements.  */
static void grow_prime_cache(size_t need)
{
    if (need <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/*-------------  Small prime list for trial division  -------------*/
#define SMALL_PRIME_LIMIT 5000
static uint32_t small_primes[SMALL_PRIME_LIMIT];
static size_t small_prime_cnt = 0;
static bool small_primes_inited = false;

/* Initialise the list of primes <= SMALL_PRIME_LIMIT.  */
static void init_small_primes(void)
{
    if (small_primes_inited)
        return;
    bool sieve[SMALL_PRIME_LIMIT + 1];
    for (size_t i = 0; i <= SMALL_PRIME_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= SMALL_PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                sieve[j] = false;
    for (size_t i = 2; i <= SMALL_PRIME_LIMIT; ++i)
        if (sieve[i]) {
            small_primes[small_prime_cnt++] = (uint32_t)i;
        }
    small_primes_inited = true;
}

/* Test whether x is prime using trial division by the pre‑computed
   small prime list.  The argument is a 64‑bit integer; the result is
   wrapped to bool.  */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * (uint64_t)p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/*-------------  Ensure that at least n primes are cached  -------------*/
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    init_small_primes();

    /* Initialise with the first term if cache is empty. */
    if (prime_cnt == 0) {
        grow_prime_cache(1);
        prime_cache[0] = (seq_t)173;   /* a(1) = 173 */
        prime_cnt = 1;
    }

    /* Generate further primes of the form 18 (mod 31).  */
    while (prime_cnt < (size_t)n) {
        uint64_t last   = (uint64_t)prime_cache[prime_cnt - 1];
        uint64_t candidate = last + 31;        /* preserve the congruence */

        while (!is_prime_uint64(candidate))
            candidate += 31;

        grow_prime_cache(prime_cnt + 1);
        prime_cache[prime_cnt] = (seq_t)candidate;
        ++prime_cnt;
    }
}

/*-------------  Public function required by the test harness  -------------*/
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* defined only for n >= 1 in OEIS */
    ensure_primes(n);
    return prime_cache[n - 1];
}
