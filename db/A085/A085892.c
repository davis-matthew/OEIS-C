/*
 * A085892  Group the natural numbers such that the product of the n-th group
 *          has n prime divisors: (1), (2), (3,4), (5,6), (7,8,9,10),
 *          (11,12,13,14), (15,16,17,18,19,20,21), ... Sequence contains the
 *          product of the terms of the groups.
 * https://oeis.org/A085892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A085892
    Computed by grouping consecutive natural numbers
    such that the product of group n has exactly n distinct prime factors.
    a(0) = 1 (product of group 0: (1))
    For n ≥ 1, find the minimal consecutive segment starting after the
    last number of the previous group whose product has n distinct prime
    factors; return that product. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* maximum prime used for sieving (large enough for expected numbers) */
#define MAX_PRIME 200000

/* global tables for prime data, built once */
static uint32_t *spf = NULL;                 /* smallest prime factor for each number ≤ MAX_PRIME */
static int      *prime_list = NULL;          /* list of primes up to MAX_PRIME */
static size_t    prime_cnt = 0;               /* number of primes */
static uint16_t *prime_index = NULL;         /* map prime → (index+1) in prime_list, 0 for non‑prime */
static uint32_t *prime_stamp = NULL;         /* stamp used to avoid clearing the seen array */
static uint32_t  global_stamp = 0;           /* increasing stamp value */

/* initialise the tables used for factorisation */
static void init_prime_tables(void)
{
    if (spf != NULL) return;   /* already initialised */

    /* smallest prime factor array */
    spf = (uint32_t *)calloc((size_t)MAX_PRIME + 1, sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);

    /* prime list (reserve enough space) */
    prime_list = (int *)malloc((size_t)(MAX_PRIME / 10 + 1) * sizeof(int));
    if (!prime_list) exit(EXIT_FAILURE);

    /* sieve for spf and list of primes */
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            prime_list[prime_cnt++] = i;
            if ((int64_t)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* index mapping for quick lookup of a prime's list position */
    prime_index = (uint16_t *)calloc((size_t)MAX_PRIME + 1, sizeof(uint16_t));
    if (!prime_index) exit(EXIT_FAILURE);
    for (size_t k = 0; k < prime_cnt; ++k) {
        prime_index[prime_list[k]] = (uint16_t)(k + 1);   /* store idx+1, 0 means not prime */
    }

    /* stamp array for seen‑prime marking */
    prime_stamp = (uint32_t *)calloc(prime_cnt, sizeof(uint32_t));
    if (!prime_stamp) exit(EXIT_FAILURE);
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* the last integer that was included in the previous group */
    static uint64_t prev_end = 0;

    /* initialise tables if this is the first call */
    init_prime_tables();

    /* grow cache if necessary */
    if (cache_len <= (size_t)n) {
        size_t new_cap = (cache_len == 0) ? 16 : cache_len * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = new_cap;
    }

    /* compute missing groups up to n */
    for (idx_t g = cache_len; g <= n; ++g) {

        /* ---- group 0: product of (1) ---- */
        if (g == 0) {
            cache[0] = 1;
            prev_end = 1;          /* last number used in group 0 */
            cache_len = 1;
            continue;
        }

        /* ---- build group g ---- */
        seq_t prod = 1;                 /* product of current group */
        uint32_t distinct_cnt = 0;      /* number of distinct primes seen */
        uint32_t stamp = ++global_stamp;/* unique stamp for this group */

        /* dynamic array for large primes (> MAX_PRIME) that appear in the group */
        uint64_t *large_primes = NULL;
        size_t large_cap = 0, large_len = 0;

        uint64_t start = prev_end + 1;  /* first integer of the current group */
        uint64_t cur = start;

        while (true) {
            uint64_t y = cur;

            /* factorise y using small primes */
            while (y > 1 && y <= (uint64_t)MAX_PRIME) {
                uint32_t p = spf[y];
                size_t idx = prime_index[p] - 1;  /* p is guaranteed prime */
                if (prime_stamp[idx] != stamp) {
                    prime_stamp[idx] = stamp;
                    ++distinct_cnt;
                }
                while (y % p == 0) y /= p;
            }

            /* remaining factor (if any) is a large prime */
            if (y > 1) {
                bool already = false;
                for (size_t i = 0; i < large_len; ++i)
                    if (large_primes[i] == y) { already = true; break; }

                if (!already) {
                    if (large_len == large_cap) {
                        large_cap = large_cap ? large_cap * 2 : 8;
                        large_primes = (uint64_t *)realloc(large_primes, large_cap * sizeof(uint64_t));
                        if (!large_primes) exit(EXIT_FAILURE);
                    }
                    large_primes[large_len++] = y;
                    ++distinct_cnt;
                }
            }

            /* multiply into the product */
            prod = prod * (seq_t)cur;

            /* stop when we have enough distinct primes */
            if (distinct_cnt >= (uint32_t)g) break;
            ++cur;
        }

        free(large_primes);          /* free temporary large‑prime list */

        cache[g] = prod;             /* store result of this group */
        prev_end = cur;              /* prepare start for next group */
        ++cache_len;
    }

    return cache[(size_t)n];
}
