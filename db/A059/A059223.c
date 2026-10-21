/*
 * A059223  Primes p such that x^37 = 2 has no solution mod p.
 * https://oeis.org/A059223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A059223: primes p such that x^37 = 2 has no solution mod p.
   For a prime p, a solution exists iff p ≡ 1 (mod 37) and
   2^((p-1)/37) ≡ 1 (mod p).  
   Hence the required primes are those with p ≡ 1 (mod 37) for
   which 2^((p-1)/37) mod p ≠ 1. 
*/

#include <stdint.h>
#include <stdlib.h>

/* ---------------------------------------------------------- */
/*  Helpers: fast modular exponentiation (uint64_t only)      */
/* ---------------------------------------------------------- */

static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------------------------------------------------------- */
/*  Simple prime generation (trial division)                 */
/* ---------------------------------------------------------- */

static uint64_t *prime_list = NULL;   /* uint64_t array (0..primes_cnt-1) */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static void add_prime_uint64(uint64_t p)
{
    if (primes_cnt == primes_cap)
    {
        size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                 /* memory failure – terminate */
        prime_list = tmp;
        primes_cap = new_cap;
    }
    prime_list[primes_cnt++] = p;
}

static int is_prime_candidate_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1ULL) == 0) return 0;        /* skip even numbers >2 */

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        uint64_t p2 = p * p;
        if (p2 > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

static uint64_t next_prime_uint64(uint64_t last)
{
    uint64_t candidate = last + 1;
    if (candidate < 2) candidate = 2;
    if (candidate == 2)
    {
        add_prime_uint64(candidate);
        return candidate;
    }
    if ((candidate & 1ULL) == 0) candidate++;          /* make it odd */

    while (1)
    {
        if (is_prime_candidate_uint64(candidate))
        {
            add_prime_uint64(candidate);
            return candidate;
        }
        candidate += 2;
    }
}

/* ---------------------------------------------------------- */
/*  Cache for sequence terms (uint64_t)                       */
/* ---------------------------------------------------------- */

static uint64_t *term_cache = NULL;   /* holds at least terms_cnt elements */
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

/* ensure terms_cache can hold at least n elements */
static void grow_terms_cache(size_t n)
{
    if (terms_cap >= n) return;
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap;
    while (new_cap < n) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(term_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    term_cache = tmp;
    terms_cap = new_cap;
}

/* ---------------------------------------------------------- */
/*  Sequence function                                         */
/* ---------------------------------------------------------- */

static uint64_t last_prime_checked = 1;   /* last prime examined by the sequence generator */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, with OK fallback */

    /* Initialise prime list with the first prime if empty          */
    if (primes_cnt == 0)
        add_prime_uint64(2);

    /* Grow term cache so we can store at least n terms              */
    grow_terms_cache(n);

    /* Generate terms until we have at least n of them               */
    while (terms_cnt < n)
    {
        last_prime_checked = next_prime_uint64(last_prime_checked);

        /* We are only interested in primes ≡ 1 (mod 37)              */
        if ((last_prime_checked - 1) % 37ULL != 0ULL)
            continue;

        /* Check whether 2 is a 37‑th power modulo this prime */
        uint64_t exp = (last_prime_checked - 1ULL) / 37ULL;
        uint64_t power = powmod_uint64(2ULL, exp, last_prime_checked);

        if (power == 1ULL)
            continue;        /* 2 is a 37‑th power – skip this prime */

        /* Store the prime in the cache                                 */
        term_cache[terms_cnt++] = last_prime_checked;
    }

    /* Return the nth term (1‑based indexing)                         */
    return (seq_t)term_cache[n - 1];
}
