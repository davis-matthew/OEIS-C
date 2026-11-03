/*
 * A092395  Primes occurring as divisors of fibonacci(p) with p prime.
 * https://oeis.org/A092395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 561, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 884 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime, mem, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n‑th prime number that occurs as a divisor of
 * Fibonacci(p) for some prime p.  
 * The implementation generates primes q in increasing order,
 * computes the smallest index r(q) such that q | Fib(r), and
 * keeps q if r(q) is itself prime.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static void fib_mod_u64(idx_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    /* Fast doubling for Fibonacci modulo 'mod'. */
    if (n == 0) { *fn = 0; *fn1 = 1; return; }

    uint64_t a = 0, b = 1;            /* a = F(k), b = F(k+1) */
    int top = 63 - __builtin_clzll(n);

    for (int i = top; i >= 0; --i) {
        /* Compute F(2k) and F(2k+1) */
        uint64_t t = (2 * b) % mod;
        if (t < a) t += mod;
        uint64_t temp = t - a;          /* (2*b - a) % mod */
        uint64_t c = (a * temp) % mod;  /* F(2k)  */
        uint64_t d = (a * a % mod + b * b % mod) % mod; /* F(2k+1) */

        a = c;
        b = d;

        if ((n >> i) & 1) {            /* Move to k+1 */
            uint64_t e = (a + b) % mod; /* F(2k+2) */
            a = b;
            b = e;
        }
    }
    *fn = a;
    *fn1 = b;
}

static uint64_t rank_prime(uint64_t q, const uint64_t *primes, size_t primes_cnt)
{
    /* Special cases */
    if (q == 2) return 3;     /* F(3) = 2 */
    if (q == 5) return 5;     /* F(5) = 5 */

    /* Legendre symbol (5|q) */
    uint64_t leg = powmod_u64(5, (q - 1) >> 1, q);
    int symbol = (leg == 1) ? 1 : -1;   /* for odd prime q, leg is 1 or q-1 */

    uint64_t M = (symbol == 1) ? (q - 1) : (2 * (q + 1));
    uint64_t d = M;
    uint64_t tmp = M;

    for (size_t i = 0; i < primes_cnt && primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            /* Reduce d by p while F(d/p) == 0. */
            while (d % p == 0) {
                uint64_t cand = d / p;
                uint64_t f, fn1;
                fib_mod_u64(cand, q, &f, &fn1);
                if (f == 0) d = cand;
                else break;
            }
        }
    }
    if (tmp > 1) {                 /* Remaining prime factor > sqrt(tmp) */
        uint64_t p = tmp;
        while (d % p == 0) {
            uint64_t cand = d / p;
            uint64_t f, fn1;
            fib_mod_u64(cand, q, &f, &fn1);
            if (f == 0) d = cand;
            else break;
        }
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Static storage for generated terms */
    static seq_t *terms   = NULL;
    static size_t len     = 0;
    static size_t cap     = 0;

    /* Prime table used both for candidate primes q and factorisation of ranks */
    static uint64_t *prime_table = NULL;
    static size_t prime_cnt      = 0;
    static uint64_t prime_limit  = 0;
    static size_t prime_iter     = 0;

    /* Initialise prime table lazily */
    if (!prime_table) {
        prime_limit = 200000;          /* Initial limit */
        build_primes_upto(prime_limit, &prime_table, &prime_cnt);
    }

    /* Generate terms until we reach the requested index n */
    while (len < n) {
        /* Extend the prime table if needed */
        if (prime_iter >= prime_cnt) {
            /* Double the limit */
            prime_limit *= 2;
            free(prime_table);             /* Free old table */
            build_primes_upto(prime_limit, &prime_table, &prime_cnt);
            prime_iter = 0;
        }

        uint64_t q = prime_table[prime_iter++];

        /* Compute rank of apparition of q */
        uint64_t r = rank_prime(q, prime_table, prime_cnt);

        /* Include q only if the rank is prime */
        if (is_prime(r)) {
            seq_t val = (seq_t) q;
            append_candidate(&terms, &len, &cap, val);
        }
    }

    return terms[n - 1];
}
