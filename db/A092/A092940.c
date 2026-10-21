/*
 * A092940  a(n) = largest prime p such that 2*prime(n) - p is prime.
 * https://oeis.org/A092940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that 2*prime(n) - p is prime
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   estimate_upper_prime, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/isPrime.h"    /* bool isPrime(idx_t x); */
#include "../utils/estimate_upper_prime.h"

static idx_t    max_n_needed = 0;     /* greatest n processed so far   */
static idx_t    *primes = NULL;      /* array of primes found          */
static size_t   primes_cnt = 0;      /* number of primes in primes[]   */
static size_t   primes_cap = 0;      /* capacity of primes[]           */
static idx_t    sieve_limit = 0;     /* current upper bound of sieve   */

/* Build the sieve of Eratosthenes up to at least the given nth prime. */
static void rebuild_sieve(idx_t required_n)
{
    idx_t limit = estimate_upper_prime(required_n);
    if (limit <= sieve_limit) return;        /* already enough */

    bool *is_comp = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_comp) return;                    /* allocation failed */
    memset(is_comp, 0, (size_t)(limit + 1) * sizeof(bool));

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_comp[j] = true;
            }
        }
    }

    if (primes) free(primes);

    primes_cap = (size_t)(limit / 10 + 100);
    primes_cnt = 0;
    primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
    if (!primes) {
        free(is_comp);
        return;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt++] = i;
        }
    }

    sieve_limit = limit;
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n ≤ 0 */

    /* Ensure we have enough primes cached for prime(n) itself. */
    if (n > max_n_needed) {
        max_n_needed = n;
        rebuild_sieve(max_n_needed);
    }

    if (!primes || primes_cnt < (size_t)n) return 0;

    idx_t pn = primes[(size_t)(n - 1)];       /* prime(n)   */
    seq_t target = (seq_t)pn * ((seq_t)2);    /* 2*prime(n) */

    /* Search downward for the largest prime p ≤ target-2
       such that target - p is also prime. */
    for (seq_t p = target - 2; p >= 2; --p) {
        if (isPrime((idx_t)p)) {
            seq_t q = target - p;
            if (q >= 2 && isPrime((idx_t)q)) {
                return p;                     /* found the largest p */
            }
        }
    }

    return 0; /* should not occur for valid n */
}
