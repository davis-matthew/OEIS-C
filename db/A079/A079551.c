/*
 * A079551  a(n) = Sum_{primes p <= n} d(p-1), where d() = A000005.
 * https://oeis.org/A079551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{primes p <= n} d(p-1) where d(k) is the number of
 *            divisors of k
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required headers that are allowed in both C11 and C++17 builds */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Cached data for the sequence  */
/* --------------------------------------------------------------------- */
static idx_t      max_n          = 0;      /* last n for which data are cached */
static uint64_t  *primes         = NULL;   /* array of primes found so far */
static size_t    primes_size     = 0;      /* allocated size of primes array */
static size_t    primes_cnt      = 0;      /* number of primes stored */

static seq_t    *prefix         = NULL;   /* cumulative sums of d(p-1) */
static size_t    prefix_size    = 0;      /* allocated size of prefix array */
static size_t    prefix_cnt     = 0;      /* number of prefix entries stored */

/* --------------------------------------------------------------------- */
/* Utility functions for dynamic array allocation                       */
/* --------------------------------------------------------------------- */
static void
ensure_prime_capacity(size_t needed)
{
    if (primes_size <= needed) {
        size_t newsize = primes_size ? primes_size * 2 : 16;
        while (newsize < needed) newsize <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, newsize * sizeof(uint64_t));
        primes = tmp;
        primes_size = newsize;
    }
}

static void
ensure_prefix_capacity(size_t needed)
{
    if (prefix_size <= needed) {
        size_t newsize = prefix_size ? prefix_size * 2 : 16;
        while (newsize < needed) newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(prefix, newsize * sizeof(seq_t));
        prefix = tmp;
        prefix_size = newsize;
    }
}

/* --------------------------------------------------------------------- */
/* Primality test using the primes that have already been found         */
/* --------------------------------------------------------------------- */
static bool
is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Divisor count of n (unsigned 64-bit) using the primes array           */
/* --------------------------------------------------------------------- */
static uint64_t
divisor_count_u64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            uint64_t exp = 0;
            do { ++exp; tmp /= p; } while (tmp % p == 0);
            res *= (exp + 1);
        }
    }
    if (tmp > 1) res *= 2;
    return res;
}

/* --------------------------------------------------------------------- */
/* Extend the cache up to (and including) index n                        */
/* --------------------------------------------------------------------- */
static void
extend_cache(idx_t n)
{
    if (n <= max_n) return;                /* nothing to do */

    for (idx_t cand = max_n + 1; cand <= n; ++cand) {
        uint64_t ucand = (uint64_t)cand;
        if (is_prime_candidate(ucand)) {   /* new prime found */
            /* store the prime */
            ensure_prime_capacity(primes_cnt + 1);
            primes[primes_cnt++] = ucand;

            /* compute d(p-1) */
            uint64_t d_val64 = divisor_count_u64(ucand - 1U);
            seq_t d_val = (seq_t)d_val64;

            /* compute new cumulative sum */
            uint64_t new_sum64 = d_val64;
            if (prefix_cnt > 0) new_sum64 += (uint64_t)prefix[prefix_cnt - 1];
            seq_t new_sum = (seq_t)new_sum64;

            /* store it */
            ensure_prefix_capacity(prefix_cnt + 1);
            prefix[prefix_cnt++] = new_sum;
        }
    }
    max_n = n;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For n < 2 there are no primes, hence the sum is 0. */
    if (n < 2) return (seq_t)0;

    /* Ensure all primes up to n have been processed. */
    extend_cache(n);

    /* All primes found so far are <= n, so prefix_cnt equals the count. */
    return (prefix_cnt > 0) ? prefix[prefix_cnt - 1] : (seq_t)0;
}
