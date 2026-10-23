/*
 * A163517  If m-th composite is the product of k1-th prime, k2-th
 *          prime,..,kr-th prime and prime=k1+k2+..+kr then set a(n)=m.
 * https://oeis.org/A163517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163517
   a(n) = index of the n-th composite m such that if m has prime factors
          at indices k1,k2,…,kr (with multiplicity) then k1+k2+…+kr
          is prime. */
/* This implementation builds the list of composites on demand and
   caches the results. It uses a dynamic list of primes and a
   simple trial‑division factorisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* -------------------------------------------------------- */
/* dynamic array of primes (1‑based indices)                */
/* -------------------------------------------------------- */
static uint64_t *prime_list   = NULL;   /* primes in ascending order   */
static size_t   prime_count   = 0;      /* number of stored primes      */
static size_t   prime_cap     = 0;      /* allocated capacity           */

/* Allocate or grow prime_list. */
static void expand_primes(void)
{
    if (prime_cap == 0) {
        prime_cap = 8;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else {
        prime_cap *= 2;
        prime_list = (uint64_t *)realloc(prime_list,
                                         prime_cap * sizeof(uint64_t));
    }
}

/* Test whether x is prime using the primes list (trial division). */
static bool is_prime_trial(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure prime_list contains all primes ≤ limit. Add until the last prime
   is at least limit. */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_count == 0) {
        /* initialise with 2 */
        expand_primes();
        prime_list[0] = 2;
        prime_count = 1;
    }
    if (prime_list[prime_count - 1] >= limit) return;

    uint64_t candidate = prime_list[prime_count - 1] + 1;
    if (candidate % 2 == 0) ++candidate;
    while (candidate <= limit) {
        bool ok = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                ok = false; break;
            }
        }
        if (ok) {
            if (prime_count == prime_cap) expand_primes();
            prime_list[prime_count++] = candidate;
        }
        candidate += 2;
    }
}

/* Binary search for prime p in prime_list (1‑based index).
   The list is guaranteed to contain p. */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) {
            return (idx_t)(mid + 1);  /* 1‑based */
        } else if (prime_list[mid] < p) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return 0; /* shouldn't happen when p is a prime factor */
}

/* -------------------------------------------------------- */
/* factorisation of n into primes (max 20 distinct factors)*/
/* -------------------------------------------------------- */
static size_t factor_simple(uint64_t n, uint64_t *pr, int *ex, size_t max_f)
{
    size_t fcnt = 0;
    if (n % 2ULL == 0ULL) {
        int e = 0;
        while (n % 2ULL == 0ULL) { n /= 2ULL; ++e; }
        if (fcnt < max_f) { pr[fcnt] = 2ULL; ex[fcnt] = e; ++fcnt; }
    }
    for (uint64_t d = 3ULL; d * d <= n && fcnt < max_f; d += 2ULL) {
        if (n % d == 0ULL) {
            int e = 0;
            while (n % d == 0ULL) { n /= d; ++e; }
            if (fcnt < max_f) { pr[fcnt] = d; ex[fcnt] = e; ++fcnt; }
        }
    }
    if (n > 1ULL && fcnt < max_f) { /* remaining prime factor */
        pr[fcnt] = n; ex[fcnt] = 1; ++fcnt;
    }
    return fcnt;
}

/* -------------------------------------------------------- */
/* caching of computed sequence values                     */
/* -------------------------------------------------------- */
static seq_t *cache      = NULL;   /* cached a(1) .. a(res_cnt-1)   */
static size_t res_cnt    = 0;      /* number of cached terms         */
static size_t res_cap    = 0;      /* capacity of cache var          */

/* current state of composite enumeration */
static uint64_t current_num              = 4;   /* next integer to examine  */
static idx_t    current_comp_index       = 1;   /* index of current_num     */

seq_t a(idx_t n)
{
    /* a(0) is unused (offset=1) */
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        res_cap = 8;
        cache = (seq_t *)malloc(res_cap * sizeof(seq_t));
        res_cnt = 0;
    }

    /* generate terms until we have at least n of them */
    uint64_t primes_buf[20];
    int      exps_buf[20];

    while (res_cnt < (size_t)n) {
        /* factorise current_num */
        size_t fcnt = factor_simple(current_num, primes_buf, exps_buf,
                                    sizeof(primes_buf)/sizeof(primes_buf[0]));

        /* determine if current_num is composite */
        bool is_comp = false;
        if (fcnt > 1) is_comp = true;
        else if (fcnt == 1 && exps_buf[0] > 1) is_comp = true;

        if (is_comp) {
            /* compute sum of prime indices with multiplicity */
            seq_t sum = 0;
            for (size_t i = 0; i < fcnt; ++i) {
                /* get index of prime factor */
                uint64_t p = primes_buf[i];
                /* ensure prime list covers p */
                ensure_primes_upto(p);
                idx_t k = prime_index(p);
                sum += (seq_t)k * (seq_t)exps_buf[i];
            }

            /* check if sum is prime (uses is_prime on idx_t) */
            if (is_prime((idx_t)sum)) {
                /* cache the composite index */
                if (res_cnt == res_cap) {
                    res_cap *= 2;
                    cache = (seq_t *)realloc(cache, res_cap * sizeof(seq_t));
                }
                cache[res_cnt++] = current_comp_index;
            }
            ++current_comp_index;   /* move to next composite number */
        }

        ++current_num; /* advance to next integer */
    }

    return cache[n - 1];
}
