/*
 * A072165  Values of Moebius function of the products of two (not necessarily
 *          distinct) primes (semiprimes or 2-almost primes, A001358).
 * https://oeis.org/A072165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3001, u16 n <= 3001, u32 n <= 3001, u64 n <= 3001,
 *            u128 n <= 3001, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = μ(p·q) for the nth semiprime p·q (p ≤ q).  
   The Möbius μ is 0 for a square of a prime, 1 otherwise (since semiprimes
   have exactly two prime factors).  The function is memoized: every term
   once computed stays in static storage, so calls may arrive in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"   /* provides _floor_isqrt_u64 */

static uint64_t *primes = NULL;          /* list of primes currently known */
static size_t primes_len   = 0;          /* number of primes stored      */
static size_t primes_cap   = 0;          /* allocated capacity of primes */
static uint64_t prime_candidate = 3;     /* next number to test primality */

static seq_t *memo   = NULL;            /* cached a(n) values (1‑based) */
static size_t memo_len = 0;             /* number of cached terms       */

/* ------------------------------------------------------------------ */
/* Ensure that all primes up to at least 'limit' are known.           */
/* ------------------------------------------------------------------ */
static void
ensure_primes_upto(uint64_t limit)
{
    /* initialise list with the first prime (2) if needed */
    if (primes_len == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_len = 0;
        primes[primes_len++] = 2;
        prime_candidate = 3;
    }

    while (prime_candidate <= limit) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > prime_candidate)
                break;
            if (prime_candidate % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes,
                                             primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = prime_candidate;
        }
        ++prime_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Checks whether x is a semiprime. If so, returns true and sets *p_out
   *q_out to its distinct prime factors (possibly equal).              */
/* ------------------------------------------------------------------ */
static bool
is_semiprime(uint64_t x, uint64_t *p_out, uint64_t *q_out)
{
    uint64_t limit_p = _floor_isqrt_u64(x) + 1;
    ensure_primes_upto(limit_p);

    uint64_t p = 0, q = 0;
    /* find smallest prime divisor p */
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t pr = primes[i];
        if ((uint64_t)pr * (uint64_t)pr > x)
            break;
        if (x % pr == 0) {
            p = pr;
            q = x / pr;
            break;
        }
    }
    if (p == 0)                    /* x is prime */
        return false;
    /* check that the other factor q is prime */
    uint64_t limit_q = _floor_isqrt_u64(q) + 1;
    ensure_primes_upto(limit_q);

    bool q_prime = true;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t pr = primes[i];
        if ((uint64_t)pr * (uint64_t)pr > q)
            break;
        if (q % pr == 0) {
            q_prime = false;
            break;
        }
    }
    if (!q_prime)
        return false;
    *p_out = p;
    *q_out = q;
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static uint64_t current = 4;  /* smallest candidate to test for semiprime */

    /* grow memo table until it contains a(n) */
    while (memo_len <= (size_t)n) {
        /* find next semiprime */
        while (true) {
            uint64_t p, q;
            if (is_semiprime(current, &p, &q)) {
                seq_t val = (p == q) ? (seq_t)0 : (seq_t)1;
                /* grow memo */
                if (memo_len == 0) {
                    memo = (seq_t *)malloc(sizeof(seq_t));
                    memo_len = 0;
                } else {
                    memo = (seq_t *)realloc(memo, sizeof(seq_t) * (memo_len + 1));
                }
                memo[memo_len++] = val;
                break;
            }
            ++current;
        }
        ++current;
    }
    return memo[(size_t)n - 1];
}
