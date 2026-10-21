/*
 * A050715  Inserting a digit '5' between adjacent digits of n makes a prime.
 * https://oeis.org/A050715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth natural number n (increasing order) such that
   inserting the digit '5' between every pair of adjacent decimal digits
   of n produces a prime number.  The first term is a(1)=11. */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Modular multiplication: a*b mod m (using 128‑bit intermediate) */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* Modular exponentiation: base^exp mod mod */
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin for 64‑bit integers.
   Uses the base set valid for all n < 2^64. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                return false;
    if (n < 4)                return true;   /* 2,3 */
    if ((n & 1) == 0)         return false;  /* even */

    /* small primes division to skip obvious composites */
    static const uint32_t small_primes[] = {
        3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u, 31u, 37u
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* witnesses that certify primality for 64‑bit numbers */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* Test whether the number resulting from inserting '5' between all
   adjacent digits of n is prime.  n fits in 64 bits, and the inserted
   number also fits in 64 bits. */
static bool inserted_is_prime(uint64_t n)
{
    /* collect digits of n in reverse order */
    uint8_t dig[20];                /* enough for up to 64‑bit numbers */
    size_t dcnt = 0;
    uint64_t tmp = n;
    while (tmp) {
        dig[dcnt++] = tmp % 10;
        tmp /= 10;
    }
    if (dcnt <= 1)          /* no adjacent pair */
        return false;

    /* build the inserted number: most significant digit first */
    uint64_t res = dig[dcnt - 1];
    for (int i = (int)dcnt - 2; i >= 0; --i) {
        res = res * 10 + 5;
        res = res * 10 + dig[i];
    }
    return is_prime_uint64(res);
}

/* --------------------------------------------------------------------- */
/* Dynamic array to cache computed terms.                                 */
/* --------------------------------------------------------------------- */
static seq_t *cache_terms = NULL;
static size_t cache_len  = 0;     /* how many terms stored */
static size_t cache_cap  = 0;     /* allocated capacity */
static uint64_t next_candidate = 11;   /* next integer to test */

/* append a term to the cache */
static void cache_append(seq_t term)
{
    if (cache_len == cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        seq_t *newptr = (seq_t *)realloc(cache_terms, newcap * sizeof(seq_t));
        if (!newptr) abort();   /* out of memory */
        cache_terms = newptr;
        cache_cap   = newcap;
    }
    cache_terms[cache_len++] = term;
}

/* --------------------------------------------------------------------- */
/*  Main function required by the specification.                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined but safe */

    /* Fill cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (inserted_is_prime(cand)) {
            cache_append((seq_t)cand);
        }
    }
    return cache_terms[n - 1];
}
