/*
 * A124591  Primes p such that q-p <= 8, where q is the next prime after p.
 * https://oeis.org/A124591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A124591
 *  a(n) = largest prime p such that the next prime q satisfies q - p <= 8
 *  (i.e. primes p where the following prime is at most 8 larger)
 *  The sequence starts: 2,3,5,7,11,13,17,19,23,29,...
 *  (offset 1)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t u64;
typedef size_t usize;

/*==========================================================================*/
/*  Internal prime generator and cache                                       */
/*==========================================================================*/
static seq_t *prime_cache = NULL;      /* cache of qualifying primes      */
static idx_t  cache_len   = 0;          /* number of cached values         */
static idx_t  cache_cap   = 0;          /* capacity of prime_cache         */

static u64 *primes     = NULL;          /* list of all primes found so far */
static usize primes_len = 0;             /* current number of primes        */
static usize primes_cap = 0;             /* capacity of primes array        */

static u64  next_candidate = 2;         /* next number to test for prime   */
static u64  last_prime     = 0;         /* previous prime found            */

/*----- grow dynamic array helpers ---------------------------------------*/
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prime_cache = tmp;
    cache_cap   = new_cap;
}

static void add_prime(u64 p)
{
    if (primes_len == primes_cap) {
        usize new_cap = primes_cap ? primes_cap * 2 : 8;
        u64 *tmp = (u64 *)realloc(primes, new_cap * sizeof(u64));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/*----- primality test using previously found primes ---------------------*/
static bool is_prime(u64 n)
{
    if (n < 2) return false;
    for (usize i = 0; i < primes_len; ++i) {
        u64 p = primes[i];
        if (p > n / p) break;           /* p * p > n  */
        if (n % p == 0) return false;
    }
    return true;
}

/*==========================================================================*/
/*  Public sequence function                                                */
/*==========================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1, undefined for 0 */

    /* generate until we have at least n cached values */
    while (cache_len < n) {
        /* find next prime */
        while (true) {
            if (is_prime(next_candidate)) {
                u64 p = next_candidate;
                add_prime(p);
                /* check previous prime */
                if (last_prime != 0 && (p - last_prime) <= 8) {
                    grow_cache(cache_len + 1);
                    prime_cache[cache_len++] = (seq_t)last_prime;
                }
                last_prime = p;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    return prime_cache[n - 1];
}
