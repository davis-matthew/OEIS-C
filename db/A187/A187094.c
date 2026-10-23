/*
 * A187094  Good primes (version 3): p such that p-1 has a prime factor
 *          between (log p)^2 and 2 (log p)^2.
 * https://oeis.org/A187094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th good prime (p such that * p-1 has a prime factor in
 *            [ (log p)^2 , 2 (log p)^2 ])
 * Range:     u8 n <= 7, u16 n <= 831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic and a small double for (log p).  */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* provides is_prime_small(idx_t) */
#include "../utils/int_sqrt.h"        /* provides int_sqrt(idx_t) */

/* ------------------------------------------------------------------ */
/*  Static state for prime generation                                 */
static idx_t    primes_cap   = 0;
static idx_t    primes_cnt   = 0;
static idx_t  *prime_list   = NULL;

static idx_t   good_cap     = 0;
static idx_t   good_cnt     = 0;
static idx_t  *good_list    = NULL;

static idx_t   last_checked = 1;   /* last candidate checked for primality */

/* ------------------------------------------------------------------ */
/*  Ensure that the prime list has enough capacity                    */
static void extend_prime_list(void)
{
    if (primes_cnt >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 128;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        prime_list = tmp;
        primes_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure that the good prime list has enough capacity                */
static void extend_good_list(void)
{
    if (good_cnt >= good_cap) {
        size_t new_cap = good_cap ? good_cap * 2 : 128;
        idx_t *tmp = (idx_t *)realloc(good_list, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        good_list = tmp;
        good_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Add a proven prime to the list                                    */
static void add_prime(idx_t p)
{
    extend_prime_list();
    prime_list[primes_cnt++] = p;
}

/* ------------------------------------------------------------------ */
/*  Test primality by trial division on the current prime list       */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    idx_t limit = (idx_t)int_sqrt(n);
    for (size_t i = 0; i < primes_cnt && prime_list[i] <= limit; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Generate the next prime greater than the last checked value       */
static idx_t next_prime(void)
{
    idx_t cand = last_checked + 1;
    if (cand <= 2) cand = 2;
    if ((cand & 1) == 0 && cand > 2) cand++;   /* skip even numbers */

    while (true) {
        /* ensure we have primes large enough to test up to sqrt(cand) */
        while (primes_cnt > 0 && prime_list[primes_cnt - 1] * prime_list[primes_cnt - 1] <= cand)
            ; /* primes list already contains all needed primes */

        if (is_prime_candidate(cand)) {
            add_prime(cand);
            last_checked = cand;
            return cand;
        }
        cand += 2;   /* next odd candidate */
    }
}

/* ------------------------------------------------------------------ */
/*  Check whether a prime p is a good prime                           */
static bool is_good_prime(idx_t p)
{
    /* Compute the logarithmic interval for prime factors of p-1   */
    double ln_p   = log((double)p);
    double low_d  = ln_p * ln_p;
    double high_d = 2.0 * low_d;
    idx_t low  = (idx_t)ceil(low_d);
    idx_t high = (idx_t)floor(high_d);

    idx_t tmp = p - 1;
    /* Trial division using the current prime list               */
    for (size_t i = 0; i < primes_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        idx_t q = prime_list[i];
        if (tmp % q == 0) {
            if (q >= low && q <= high) return true;
            while (tmp % q == 0) tmp /= q;
        }
    }
    /* Remaining factor, if any, is prime                        */
    if (tmp > 1 && tmp >= low && tmp <= high) return true;
    return false;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                          */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    /* Ensure the prime and good-prime lists have been initialized */
    if (primes_cnt == 0) add_prime(2);    /* start with first prime 2 */
    if (good_cnt == 0) {
        if (prime_list[0] == 2) good_cnt = 0; /* 2 is not good */
        else good_cnt = 1; /* dummy */
    }

    /* Generate good primes until we have at least n of them         */
    while (good_cnt < n) {
        idx_t p = next_prime();        /* get next prime candidate      */
        if (is_good_prime(p)) {
            extend_good_list();
            good_list[good_cnt++] = p;
        }
    }

    return (seq_t)good_list[n - 1];
}
