/*
 * A180458  Largest palindromic number <= n-th-prime.
 * https://oeis.org/A180458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Formula: a(n) = largest palindrome <= nth prime
 *
 * The function computes the nth prime using a sieve up to an
 * estimate produced by _prime_limit_for_idx.  Then it searches
 * downward from that prime until a decimal palindrome is found.
 * Memoisation of the prime list allows repeated calls in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_palindrome.h"

static uint64_t *g_primes = NULL;
static size_t g_primesCnt = 0;

/* Find the largest palindrome <= x (decimal) */
static seq_t largest_pal_leq(seq_t x)
{
    seq_t cur = x;
    while (!is_palindrome(cur, 10)) {
        if (cur == 0) break;   /* prevent wrap‑around of unsigned type */
        --cur;
    }
    return cur;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n=0; return 0 to satisfy type */
    }

    /* Only sieve new primes if we don't yet have n of them */
    if (g_primes == NULL || g_primesCnt < n) {
        if (g_primes) {
            free(g_primes);
            g_primes = NULL;
        }

        /* Estimate an upper bound for the n-th prime */
        idx_t limit = _prime_limit_for_idx(n);
        size_t cnt = 0;
        uint64_t *tmp = NULL;

        build_primes_upto(limit, &tmp, &cnt);
        g_primes = tmp;
        g_primesCnt = cnt;

        /* If the estimate was too small, redo with a larger bound */
        while (g_primesCnt < n) {
            limit = _prime_limit_for_idx(n + 10);  /* add a small safety margin */
            if (g_primes) {
                free(g_primes);
            }
            build_primes_upto(limit, &tmp, &cnt);
            g_primes = tmp;
            g_primesCnt = cnt;
        }
    }

    uint64_t p = g_primes[n - 1];          /* nth prime (1‑based) */
    seq_t pseq = (seq_t)p;                 /* cast to seq_t for palindrome test */
    return largest_pal_leq(pseq);
}
